
# if(refr_mode == 'low') {
#     u_abs <- -120 # mV
#     u_r <- -50#-50 # mV
#     trf <- 2.25 # ms
#     trs <- 2 # ms
#     dr <- 1 # ms
# } else 
# if(refr_mode == 'middle') {
#     u_abs <- -150 # mV
#     u_r <- -50#-50 # mV
#     trf <- 3.25 # ms
#     trs <- 3 # ms
#     dr <- 1 # ms
# } else
# if(refr_mode == 'high') {
#     u_abs <- -250 # mV
#     u_r <- -70#-50 # mV
#     trf <- 5.25 # ms
#     trs <- 5 # ms
#     dr <- 3 # ms
# } else {
#     cat(sprintf("Can't find refration mode %s\n", refr_mode))
# }

epsp2 <- Vectorize(function(s) {
  if((s<0)||(s == Inf)) { return(0) }
  e0*(exp(-s/tm)-exp(-s/ts))
})
epsp <- Vectorize(function(s) {
  if((s<0)||(s == Inf)) { return(0) }
  exp(-s/tm)
})

# absolute(-100 mV) refractoriness with exponential restore
#nu <- Vectorize(function(s) {
# if(s<0) { return(0)}
# if(s<dr) { return(u_abs) }
# u_abs*exp(-(s+dr)/trf)+u_r*exp(-s/trs)
#})

#t <- seq(0, 50, by=0.1)
#plot(t, nu(t-25), type="l")

# stochastic threshold g(u):

g <- Vectorize(function(u) {
  ans=(beta/alpha)*(log(1+exp(alpha*(tr-u))) -alpha*(tr-u))   
  return(ans)
})

u_srm <- function(t, x, y, w) {
  e_syn <- 0
  for(i in 1:length(x)) {
    e_syn <- e_syn + w[i]*sum(epsp(t-x[[i]]))
  }
  u_rest + e_syn + sum(nu(t-y))
}

probf = function(u) {
  pr + (u -u_rest)*gain_factor
}

a_suppr = Vectorize(function(s) {
  if(s<0) return(1)
  return(1- exp(-s/ta))
})

g0=1
r0=9.25
beta=0.5
u_tr=15

g2 = Vectorize(function(u) {
    g0+r0*log(1+exp(beta*(u-u_tr)))
})

u=seq(0,100, length.out=1000)
plot(u, g2(u),type="l", xlim=c(0,20), ylim=c(0,20), lwd=2.5, las=1)

e0=4
tm=10 
ts=5
u=seq(0,100, length.out=1000)
plot(u, epsp2(u),type="l", xlim=c(0,100), ylim=c(0,1), lwd=2.5, las=1, xlab="t, мс", ylab="I")

e0=2.60
tm=30 
ts=10
u=seq(0,100, length.out=1000)
lines(u, epsp2(u),lwd=2.5, las=1)



