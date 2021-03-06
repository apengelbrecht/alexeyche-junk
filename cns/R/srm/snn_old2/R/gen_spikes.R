#!/usr/bin/RScript

TSNeurons <- setRefClass("TSNeurons", fields = list(M = "vector", patterns = "list", ids_prop="vector"), 
                                    methods = list(
                                    initialize = function(M, ids_c=NULL) {
                                      if(is.null(ids_c)) {
                                        ids_prop <<- get_unique_ids(M)
                                      } else {
                                        ids_prop <<-- ids_c
                                      }
                                      patterns <<- list()
                                      M <<- M
                                    },
                                    len = function() {
                                      return(.self$M)
                                    },
                                    loadPattern = function(rawdata, pattDur, label, simdt, lambda=4, hb=NULL, lb=NULL) {
                                      l <- length(patterns)
                                      
                                      patt <- list(pattDur=pattDur, dt=pattDur/length(rawdata), rawdata=rawdata, len=length(rawdata), label=label)
                                      
                                      if(is.null(hb)) hb <- max(rawdata)
                                      if(is.null(lb)) lb <- min(rawdata)
                                      

                                      patt_dt <- 0
                                      approx_data = rep(NA, pattDur/simdt)                                      
                                      for(ri in 1:patt$len) {
                                        patt_dt <- patt_dt + pattDur/patt$len
                                        ct = ceiling(signif(patt_dt/simdt, digits=5))                                        
                                        approx_data[ct] = rawdata[ri]
                                      }
                                      
                                      approx_data = na.approx(approx_data)
                                      approx_data = approx_data[!is.na(approx_data)]
                                      
                                      gen_spikes = vector("list", M)
                                                                       
                                      neurons_rate = rep(0, M)                                      
                                      dt <- (hb-lb)/(M-1)
                                      t = 0                                       
                                      for(i_val in 1:length(approx_data)) {
                                        if(is.na(approx_data[i_val])) {
                                          fired=-1
                                        } else  {
                                          fired <- floor((approx_data[i_val]-lb)/dt)+1
                                        }
                                        
                                        if((fired>0) && (neurons_rate[fired] == 0)) {                                        
                                          gen_spikes[[fired]] = c(gen_spikes[[fired]], t)
                                          neurons_rate[fired] = lambda 
                                        }                                        
                                        refr_neurons = neurons_rate > 0
                                        neurons_rate[refr_neurons] = neurons_rate[refr_neurons]-simdt
                                        neurons_rate[neurons_rate < 0] = 0
                                        t = t + simdt
                                      }
                                      null_ids = which( sapply(gen_spikes, is.null) == TRUE )
                                      gen_spikes[null_ids] = blank_net(length(null_ids))
                                      patt$data <- gen_spikes
                                      patterns[[l+1]] <<- patt                    
                                    },
                                    loadPatternFromFile = function(file, pattDur, class, simdt, lambda=4, hb=NULL, lb=NULL) {
                                      rawdata <- c(read.table(file, sep=",")[,1])
                                      .self$loadPattern(rawdata,pattDur, class, simdt, lambda, hb, lb)
                                    },
                                    loadPatterns = function(dataset, pattDur, simdt, lambda=4) {
                                      dmin = max(sapply(dataset, function(x) max(x$data)))
                                      dmax = min(sapply(dataset, function(x) min(x$data)))
                                      for(d in dataset) {
                                        .self$loadPattern(d$data, pattDur, d$label, simdt, lambda, dmax, dmin)
                                      }
                                    },
                                    preCalculate = function(T0, Tmax, dt) {
                                        T0_current <- T0
                                        for(pattern in patterns) {
                                            patt_ti <- 0
                                            for(t in seq(T0_current, Tmax, by=dt)) {
                                                patt_index <- patt_ti/pattern$dt
                                                cat("t: ", t, " patt_index: ", patt_index, " patt_ti: ", patt_ti, "\n") 
                                                patt_ti <- patt_ti + dt    
                                                if(patt_index > pattern$len) {
                                                    T0_current = t
                                                    break
                                                }

                                            }
                                            if(T0_current>=Tmax) {
                                                break
                                            }
                                        }
                                    },
                                    ids = function() {
                                        return(.self$ids_prop)
                                    }))



#file <- "/home/alexeyche/prog/sim/stimuli/d1.csv"

plot_patt <- function(patt) {
    pl_mat <- NULL
    for(i in 1:ncol(patt$data)) {
        n_fired <- which(patt$data[,i] >= 0)
        x <- patt$data[n_fired,i]
        y <- n_fired
        pl_mat <- rbind(pl_mat, c(x, y))
    }
    plot(pl_mat)
}
#plot_patt(gr1$patterns[[1]])
#preCalculate(gr1$patterns[[1]], 0, 300, 0.5)
spikeMatToSpikeList <- function(m) {
    l <- list()
    for(ri in 1:nrow(m)) {
        l[[ri]] <- m[ri, m[ri, ]>=0]
    }
    return(l)
}


