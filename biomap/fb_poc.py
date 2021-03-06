

from matplotlib import pyplot as plt
import numpy as np

from util import shl, shm, shs

from fb_common import Sigmoid, Linear, get_oja_deriv, norm, Learning


lrate = 1e-03

in_size = 50
hidden_size0 = 30
hidden_size1 = 20
out_size = 10

epochs = 100

np.random.seed(9)


act = Linear()

F = -1.0 + 2.0*np.random.rand(in_size, out_size)

T = 100

x_s = np.random.randn(T, in_size)
y_t_s = act(np.dot(x_s, F))


stats = []
for lrule in (Learning.OJA_FEED,):
# for lrule in (Learning.BP, Learning.FA, Learning.OJA, Learning.OJA_FEED):
	error_acc = []
	np.random.seed(1)

	W0 = -0.1 + 0.2*np.random.rand(in_size, hidden_size0)
	W1 = -0.1 + 0.2*np.random.rand(hidden_size0, hidden_size1)
	W2 = -0.1 + 0.2*np.random.rand(hidden_size1, out_size)

	B0 = -0.1 + 0.2*np.random.randn(out_size, hidden_size0)
	B1 = -0.1 + 0.2*np.random.randn(out_size, hidden_size1)

	B0_start = B0.copy()
	B1_start = B1.copy()

	for epoch in xrange(epochs):
		errors = []
		y_acc = []
		for ti in xrange(T):
			x = x_s[ti, :]
			y_t = y_t_s[ti, :]

			a0 = np.dot(x, W0); h0 = act(a0)
			a1 = np.dot(h0, W1); h1 = act(a1)
			a2 = np.dot(h1, W2); y = act(a2)

			e = y - y_t
			error = 0.5 * np.inner(e, e)

			x1 = np.dot(e, B1); yf1 = act(x1)
			x0 = np.dot(e, B0); yf0 = act(x0)

			if lrule == Learning.BP or lrule == Learning.FA:
				if lrule == Learning.BP:
					dh1 = np.dot(W2, e) * act.deriv(a1).T
					dh0 = np.dot(W1, dh1) * act.deriv(a0).T
				else:
					dh1 = np.dot(e, B1) * act.deriv(a1)
					dh0 = np.dot(e, B0) * act.deriv(a0)

				dW0 = - np.outer(x, dh0)
				dW1 = - np.outer(h0, dh1)
				dW2 = - np.outer(h1, e)

			elif lrule == Learning.OJA or lrule == Learning.OJA_FEED:
				dW2 = - np.outer(h1, e)

				dW0 = get_oja_deriv(x, yf0, W0, act.deriv(yf0))
				dW1 = get_oja_deriv(h0, yf1, W1, act.deriv(yf1))

				dW0 = np.zeros(dW0.shape)
				dW1 = np.zeros(dW1.shape)

				if lrule == Learning.OJA_FEED:
					dB0 = get_oja_deriv(e, yf0, B0, act.deriv(yf0))
					dB1 = get_oja_deriv(e, yf1, B1, act.deriv(yf1))

					B0 += lrate * norm(dB0)
					B1 += lrate * norm(dB1)

			W0 += lrate * norm(dW0)
			W1 += lrate * norm(dW1)
			W2 += lrate * norm(dW2)

			errors.append(error)
			y_acc.append(y)
			
		# 	break
		# break
		error = sum(errors)/len(errors)
		if epoch % 5 == 0 or epoch == epochs-1:
			print "{}, Epoch {}, error {}".format(lrule, epoch, error)

		error_acc.append(error)

	stats.append(error_acc)
	y_res = np.concatenate([y_acc])


# plt.plot(stats[0], "r")
# plt.plot(stats[1], "b")
# plt.plot(stats[2], "g")
# plt.plot(stats[3], "black")
# plt.show()


