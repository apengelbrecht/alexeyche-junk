#!/usr/bin/RScript

source('sys.R')

sigmoid <- function(x) {
    1/(1+exp(-x))
}

sample_bernoulli <- function(probs) {
    dims <- dim(probs)
    matrix(rbinom(prod(dims),size=1,prob=c(probs)),dims[1],dims[2])
}

free_energy <- function(v,W,hid_bias,vis_bias) {
    n <- nrow(v)    
    - v %*% t(vis_bias) - sum.col(log(1+exp(v %*% W + rep.row(hid_bias,n) )) )
}

gibbs_hvh <- function(hid_sample,model) {
    n <- nrow(hid_sample)
    # p(v_model|h) calculate visible state with given hidden units state from positive phase
    vis.probs <- sigmoid( hid_sample %*% t(model$W) + rep.row(model$vis_bias, n) )    
    # p(h|v_model) calculate hidden state with given visible units
    hid.probs <- sigmoid( vis.probs %*% model$W + rep.row(model$hid_bias, n) )
    list(hid.probs = hid.probs,vis.probs = vis.probs)
}

contrastive_divergence <- function(hid_prob, model, iter = 10) {    
    for(it in 1:iter) {
        hid_states <- sample_bernoulli(hid_prob) # col for each hidden unit, row for each case
        c(hid_prob, vis_prob) := gibbs_hvh(hid_states,model)
    }
    return(list(hid.probs.end = hid_prob, vis.probs.end = vis_prob))
}

# p(h_j = 1 | v,w) = sigmoid(sum_j v_j * w_ij - b_j)

# o o o   | j = 3
#  o o    | i = 2
set.seed(0)
n <- 10
num.dims <- 2
err.total <- 0
params = list(e.w = 0.1, e.v = 0.1, e.h = 0.1, w_cost = 0.0002)   
initial_momentum  <- 0.5
final_momentum    <- 0.9

#data <- array(rnorm(n * num.dims), dim = c(n,num.dims))
data <- array(rep(1,n*num.dims), dim = c(n,num.dims))
v_num <-2
h_num <-3
vis_bias <- array(0,dim = c(1,v_num))
hid_bias <- array(0,dim = c(1,h_num))
W <- array(0.1*rnorm(v_num*h_num),dim=c(v_num,h_num)) # visible units for row, hidden units for col
model <- list(W = W, vis_bias = vis_bias, hid_bias = hid_bias)
W.inc <- hid_bias.inc <- vis_bias.inc <- 0

for (v in 1:length(params)) assign(names(params)[v], params[[v]])

epochs <- 100
for(epoch in 1:epochs) {            
    epoch <- 1 
    # positive phase
    # p(h|x) calculate hidden states with given visible units state
    hid_prob <- sigmoid( data %*% model$W + rep.row(model$hid_bias,n) ) # col for each hidden unit, row for each case
    
    
    # negative phase

    #vis_prob.model <- sigmoid( hid_states %*% t(model$W) + rep.row(model$vis_bias,n) )
    #hid_prob.model <- sigmoid( vis_prob.model %*% model$W + rep.row(model$hid_bias,n) )
    c(hid_prob.model, vis_prob.model) := contrastive_divergence(hid_prob, model, iter = 100000)
    
    
    err <- sum((data - vis_prob.model)^2)
    err.total <- err.total + err
    E.free.mean <- sum(free_energy(data, model$W, model$hid_bias, model$vis_bias))/n
    cat("Epoch #", epoch," err: ", err," free energy: ", E.free.mean,"\n") 
    # moment stuff 
    momentum <- final_momentum
    if (epoch <= 5) {
        momentum <- initial_momentum
    }
    
    # deravatvies over log p(v)
    # d_log_p(v) / d_W_ij
    W.inc <- momentum*W.inc + e.w*( ( t(data) %*% hid_prob - t(vis_prob.model) %*% hid_prob.model)/n - w_cost*W)
    # d_log_p(v) / d_hid_bias_j
    hid_bias.inc <- momentum*hid_bias.inc + e.h*(sum.row(hid_prob) - sum.row(hid_prob.model))/n
    # d_log_p(v) / d_vis_bias_i
    vis_bias.inc <- momentum*vis_bias.inc + e.v*(sum.row(data) - sum.row(vis_prob.model))/n
    model$W <- model$W + W.inc
    model$hid_bias <- model$hid_bias + hid_bias.inc
    model$vis_bias <- model$vis_bias + vis_bias.inc    
    for (v in 1:length(model)) assign(names(model)[v], model[[v]]) 
#}
#hist(W[1,]); hist(W[2,])
# per element explanation
#for(j in 1:h_num) {    
#    Wsum <- 0
#    for(i in 1:v_num) {        
#        Wsum <- Wsum + W[i,j] * vector[i] 
#    }
#    pos_prob_h[j] <- sigmoid(bias_h[j]+Wsum)
#}
