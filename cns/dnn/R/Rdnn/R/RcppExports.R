# This file was generated by Rcpp::compileAttributes
# Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

set.verbose.level <- function(level) {
    invisible(.Call('Rdnn_setVerboseLevel', PACKAGE = 'Rdnn', level))
}

chop.time.series <- function(l) {
    .Call('Rdnn_chopTimeSeries', PACKAGE = 'Rdnn', l)
}

chop.spikes.list <- function(l) {
    .Call('Rdnn_chopSpikesList', PACKAGE = 'Rdnn', l)
}

