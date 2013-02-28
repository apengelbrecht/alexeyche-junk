#!/usr/bin/PScript
import theano
import theano.tensor as T
from util import getch
import numpy as np
from numpy import genfromtxt
import os
from theano.tensor.shared_randomstreams import RandomStreams
import cPickle

import rpy2.robjects as ro
from rpy2.robjects.packages import importr

from rbm import RBM, RBMBinLine

    
class AutoEncoder(object):
    def __init__(self, rbms, num_out = 2):
        self.input = rbms[0].input
        self.output = T.ivector('output')
        
        self.stack = rbms 
        self.num_layers = rbms.num_layers
        self.num_vis = rbms[0].num_vis
        
        top_rbm = RBMBinLine(input = self.stack[-1].output, num_vis = self.stack[-1].num_hid, num_hid = num_out)  
                     
        self.stack.append(top_rbm)
        self.params = []
        for rbm in rbms:
            self.params.extend(rbm.params)
    
    def pretrain_fun(self, data_sh, train_params):
        funcs = self.stack.pretrain_fun(data_sh, train_params)
        learning_rate_line = train_params['learning_rate_line'] 
        num_cases = data_sh.get_value(borrow=True).shape[0]
        
        index = T.lscalar('index')  # index to a minibatch
        cost, free_en, gparam, updates = rbm.get_cost_updates(lr=learning_rate_line, num_cases=num_cases, persistent=persistent_chain, k=cd_steps)

        train_rbm_f = theano.function([index], [cost, free_en, gparam],
               updates=updates,
               givens=[(self.input, data_sh[index * batch_size: (index + 1) * batch_size])])
            
        funcs.append(train_rbm_f)
        return funcs
    
    def get_output(self):
        output = self.stack[-1].output # from the top
        for l in reversed(xrange(0,self.num_layers)):
            _, output = self.stack[l].prop_down(output)
        return output
    
    def finetune_cost(self):
        output = self.get_output()
        return T.mean(T.sum(T.sqr(self.input - output), axis=1))
    
    def finetune_fun(self, data_sh, train_params):
        batch_size = train_params['batch_size']
        learning_rate = train_params['finetune_learning_rate']

        index = T.lscalar('index')  # index to a [mini]batch
        cost = self.finetune_cost() # wrt to self.input
        gparams = T.grad(cost, self.params)
        updates = []
        for param, gparam in zip(self.params, gparams):
            updates.append((param, param - gparam * learning_rate))
        train_fn = theano.function([index], cost, updates=updates,
                                  givens=[ (self.input, data_sh[index * batch_size: (index + 1) * batch_size])])

        return train_fn

def gen_name(ae):
    name = "ae"
    for rbm in ae.stack:
        name += "_%d" % (rbm.num_hid) 
    return name        

def save_to_file(ae):
    fileName_p = open(gen_name(ae), 'wb')
    for rbm in ae.stack:
        cPickle.dump(rbm.W.get_value(borrow=True), fileName_p, -1)  # the -1 is for HIGHEST_PROTOCOL
        cPickle.dump(rbm.vbias.get_value(borrow=True), fileName_p, -1)  # .. and it triggers much more efficient
        cPickle.dump(rbm.hbias.get_value(borrow=True), fileName_p, -1)  # .. storage than numpy's default

def load_from_file(ae):
    fileName_p = open(gen_name(ae), 'r')
    for rbm in ae.stack:    
        rbm.W.set_value(cPickle.load(fileName_p), borrow=True)
        rbm.vbias.set_value(cPickle.load(fileName_p), borrow=True)
        rbm.hbias.set_value(cPickle.load(fileName_p), borrow=True)

def print_top_to_file(ae, name, data_sh, data_target, cases):
    ae_name = gen_name(ae)
    fileName = ae_name + "_" + name + ".png"

    out_line = ae.stack[-1].output
    f = theano.function([], out_line, givens=[(ae.input, data_sh[cases])])
    hid_stat = f()
    x = hid_stat[...,0].tolist()
    y = hid_stat[...,1].tolist()
    grdevices = importr('grDevices')
    grdevices.png(file=fileName, width=512, height=512)
    lab = ro.IntVector(data_target[cases].tolist())
    lab_col = ro.StrVector(map(lambda p: p == 0 and 'blue' or 'red', lab))
    lab_col.names = lab
    ro.r.plot(x,y, xlab = "x", ylab="y", type="n")
    ro.r.text(x,y,labels=lab, col = lab_col)
    grdevices.dev_off()

