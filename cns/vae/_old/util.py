
import tensorflow as tf
from matplotlib import pyplot as plt
import numpy as np
import math

def sm(*matrices, **kwargs):
    for m_id, matrix in enumerate(matrices):
        plt.subplot(1, len(matrices),m_id+1)
        plt.imshow(np.squeeze(matrix).T)
        plt.colorbar()

    if kwargs.get("file"):
        plt.savefig(kwargs["file"])
        plt.clf()
    else:
        plt.show()

def smooth(signal, sigma=0.01, filter_size=50):
    lf_filter = np.exp(-np.square(0.5-np.linspace(0.0, 1.0, filter_size))/sigma)
    return np.convolve(lf_filter, signal, mode="same")

def smooth_matrix(m, sigma=0.01, filter_size=50):
    res = np.zeros(m.shape)
    for dim_idx in xrange(m.shape[1]):
        res[:, dim_idx] = smooth(m[:, dim_idx], sigma, filter_size)
    return res

def sl(*vector, **kwargs):
    labels = kwargs.get("labels", [])
    for id, v in enumerate(vector):
        if len(labels) > 0:
            plt.plot(np.squeeze(v), label=labels[id])
        else:
            plt.plot(np.squeeze(v))

    if len(labels) > 0:
        plt.legend()

    if kwargs.get("file"):

        plt.savefig(kwargs["file"])
        plt.clf()
    else:
        plt.show()

def moving_average(a, n=3) :
    ret = np.cumsum(a, dtype=float)
    ret[n:] = ret[n:] - ret[:-n]
    v = ret[n - 1:] / n
    return np.pad(v, [0, n-1], 'constant')

def norm(data, return_denom=False):
    data_denom = np.sqrt(np.sum(data ** 2))
    data = data/data_denom
    if not return_denom:
        return data
    return data, data_denom


def outer(left_v, right_v):
    return mo.matmul(tf.expand_dims(left_v, 1), tf.expand_dims(right_v, 0))


def generate_dct_dictionary(l, size):
    p = np.asarray(xrange(l))
    filters = np.zeros((l, size))
    for fi in xrange(size):
        filters[:, fi] = np.cos((np.pi * (2 * fi + 1) * p)/(2*l))
        filters[0, fi] *= 1.0/np.sqrt(2.0)
        # filters[fi, 1:] *= np.sqrt(2/l)
    return filters * np.sqrt(2.0/l)

def xavier_init(fan_in, fan_out, const=0.5):
    """Xavier initialization of network weights.

    https://stackoverflow.com/questions/33640581/how-to-do-xavier-initialization-on-tensorflow

    :param fan_in: fan in of the network (n_features)
    :param fan_out: fan out of the network (n_components)
    :param const: multiplicative constant
    """
    low = -const * np.sqrt(6.0 / (fan_in + fan_out))
    high = const * np.sqrt(6.0 / (fan_in + fan_out))
    return tf.random_uniform((fan_in, fan_out), minval=low, maxval=high)

def xavier_vec_init(fan_in, const=1.0):
    low = -const * np.sqrt(6.0 / fan_in)
    high = const * np.sqrt(6.0 / fan_in)
    return tf.random_uniform((fan_in,), minval=low, maxval=high)

def fun(*args, **kwargs):
    assert 'nout' in kwargs, "Need output size"
    assert 'name' in kwargs, "Need name for output"

    assert len(args) > 0, "Empty args"

    nout = kwargs["nout"]
    name = kwargs["name"]

    config = kwargs.get("config", {})
    for k, v in config.iteritems():
        if not k in kwargs:
            kwargs[k] = v
    act = kwargs.get("act", tf.nn.tanh)
    use_bias = kwargs.get("use_bias", True)
    weight_factor = kwargs.get("weight_factor", 1.0)
    use_weight_norm = kwargs.get("use_weight_norm", False)
    layers_num = kwargs.get("layers_num", 1)
    reuse = kwargs.get("reuse", False)
    
    with tf.variable_scope(name, reuse=reuse) as scope:
        inputs = args

        for l_id in xrange(layers_num):
            layer_out = tf.zeros(inputs[0].get_shape().as_list()[:-1] + [nout], dtype=tf.float32)

            for idx, a in enumerate(inputs):
                a_shape = a.get_shape().as_list()

                nin = a_shape[-1]

                init = lambda shape, dtype, partition_info: xavier_init(nin, nout, const = weight_factor)
                vec_init = lambda shape, dtype, partition_info: xavier_vec_init(nout, const = weight_factor)
                bias_init = lambda shape, dtype, partition_info: np.zeros((nout,))
                if not use_weight_norm:
                    w = tf.get_variable("W{}-{}".format(l_id, idx), [nin, nout], dtype = tf.float32, initializer = init)
                    a_w = tf.matmul(a, w)
                else:
                    V = tf.get_variable("V{}-{}".format(l_id, idx), [nin, nout], dtype = tf.float32, initializer = init) #tf.uniform_unit_scaling_initializer(factor=weight_factor))
                    g = tf.get_variable("g{}-{}".format(l_id, idx), [nout], dtype = tf.float32, initializer = vec_init)

                    a_w = tf.matmul(a, V)
                    a_w = a_w * g/tf.sqrt(tf.reduce_sum(tf.square(V),[0]))

                if use_bias:
                    b = tf.get_variable("b{}-{}".format(l_id, idx), [nout], tf.float32, initializer = bias_init)
                    a_w = a_w + b

                layer_out = layer_out + a_w
            inputs = (act(layer_out),)

    return inputs[0]


def kl_divergence_gauss(p_mu, log_p_sigma, q_mu, log_q_sigma):
    return - tf.constant(0.5) + log_q_sigma - log_p_sigma + \
            (tf.exp(tf.constant(2.0) * log_p_sigma) + tf.square(p_mu - q_mu)) / (tf.constant(2.0) * tf.exp(tf.constant(2.0) * log_q_sigma))


def log_sum_exp(x, axis=None):
    x_max = tf.reduce_max(x, axis=axis, keep_dims=True)
    z = tf.log(tf.reduce_sum(tf.exp(x - x_max), axis, keep_dims=True)) + x_max
    return tf.reduce_sum(z, axis)



def gmm_neg_log_likelihood(y, mu, sigma, alpha):
    y_size = len(y.get_shape().as_list())
    mu_size = len(mu.get_shape().as_list())
    assert mu_size <= y_size, "data is smaller than model"

    # sigma = tf.exp(logsigma)

    ps = tf.exp(-((y - mu)**2)/(2.0*sigma**2))/(sigma*np.sqrt(2*math.pi))
    pin = ps * alpha
    return - tf.log(tf.reduce_sum(pin, mu_size-1))
    # log_lik = - tf.square(y - mu) / (2.0 * tf.exp(2.0 * sigma)) - 2.0 * sigma - tf.log(2.0 * np.pi)
    # return - tf.log(alpha) - log_lik
    
