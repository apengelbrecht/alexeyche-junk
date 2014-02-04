// This file was generated by Rcpp::compileAttributes
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// evalNet
SEXP evalNet(const List patterns, const List run_options, const List constants, List layers);
RcppExport SEXP snnSRM_evalNet(SEXP patternsSEXP, SEXP run_optionsSEXP, SEXP constantsSEXP, SEXP layersSEXP) {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        Rcpp::RNGScope __rngScope;
        Rcpp::traits::input_parameter< const List >::type patterns(patternsSEXP );
        Rcpp::traits::input_parameter< const List >::type run_options(run_optionsSEXP );
        Rcpp::traits::input_parameter< const List >::type constants(constantsSEXP );
        Rcpp::traits::input_parameter< List >::type layers(layersSEXP );
        SEXP __result = evalNet(patterns, run_options, constants, layers);
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}
// integrateSRM
SEXP integrateSRM(const List constants, const List int_options, const IntegerVector neuron_id, const IntegerVector neuron_id_conn, const NumericVector neuron_w, const List net);
RcppExport SEXP snnSRM_integrateSRM(SEXP constantsSEXP, SEXP int_optionsSEXP, SEXP neuron_idSEXP, SEXP neuron_id_connSEXP, SEXP neuron_wSEXP, SEXP netSEXP) {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        Rcpp::RNGScope __rngScope;
        Rcpp::traits::input_parameter< const List >::type constants(constantsSEXP );
        Rcpp::traits::input_parameter< const List >::type int_options(int_optionsSEXP );
        Rcpp::traits::input_parameter< const IntegerVector >::type neuron_id(neuron_idSEXP );
        Rcpp::traits::input_parameter< const IntegerVector >::type neuron_id_conn(neuron_id_connSEXP );
        Rcpp::traits::input_parameter< const NumericVector >::type neuron_w(neuron_wSEXP );
        Rcpp::traits::input_parameter< const List >::type net(netSEXP );
        SEXP __result = integrateSRM(constants, int_options, neuron_id, neuron_id_conn, neuron_w, net);
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}
// integrateSRM_vec
SEXP integrateSRM_vec(const List constants, const List int_options, const IntegerVector neurons_id, const List neurons_id_conn, const List neurons_w, const List net);
RcppExport SEXP snnSRM_integrateSRM_vec(SEXP constantsSEXP, SEXP int_optionsSEXP, SEXP neurons_idSEXP, SEXP neurons_id_connSEXP, SEXP neurons_wSEXP, SEXP netSEXP) {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        Rcpp::RNGScope __rngScope;
        Rcpp::traits::input_parameter< const List >::type constants(constantsSEXP );
        Rcpp::traits::input_parameter< const List >::type int_options(int_optionsSEXP );
        Rcpp::traits::input_parameter< const IntegerVector >::type neurons_id(neurons_idSEXP );
        Rcpp::traits::input_parameter< const List >::type neurons_id_conn(neurons_id_connSEXP );
        Rcpp::traits::input_parameter< const List >::type neurons_w(neurons_wSEXP );
        Rcpp::traits::input_parameter< const List >::type net(netSEXP );
        SEXP __result = integrateSRM_vec(constants, int_options, neurons_id, neurons_id_conn, neurons_w, net);
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}
// USRM
SEXP USRM(const NumericVector t, const List constants, const IntegerVector neuron_id, const IntegerVector neuron_id_conn, const NumericVector neuron_w, const List net);
RcppExport SEXP snnSRM_USRM(SEXP tSEXP, SEXP constantsSEXP, SEXP neuron_idSEXP, SEXP neuron_id_connSEXP, SEXP neuron_wSEXP, SEXP netSEXP) {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        Rcpp::RNGScope __rngScope;
        Rcpp::traits::input_parameter< const NumericVector >::type t(tSEXP );
        Rcpp::traits::input_parameter< const List >::type constants(constantsSEXP );
        Rcpp::traits::input_parameter< const IntegerVector >::type neuron_id(neuron_idSEXP );
        Rcpp::traits::input_parameter< const IntegerVector >::type neuron_id_conn(neuron_id_connSEXP );
        Rcpp::traits::input_parameter< const NumericVector >::type neuron_w(neuron_wSEXP );
        Rcpp::traits::input_parameter< const List >::type net(netSEXP );
        SEXP __result = USRM(t, constants, neuron_id, neuron_id_conn, neuron_w, net);
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}
// USRMs
SEXP USRMs(const NumericVector t, const List constants, const IntegerVector neurons_id, const List neurons_id_conn, const List neurons_w, const List net);
RcppExport SEXP snnSRM_USRMs(SEXP tSEXP, SEXP constantsSEXP, SEXP neurons_idSEXP, SEXP neurons_id_connSEXP, SEXP neurons_wSEXP, SEXP netSEXP) {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        Rcpp::RNGScope __rngScope;
        Rcpp::traits::input_parameter< const NumericVector >::type t(tSEXP );
        Rcpp::traits::input_parameter< const List >::type constants(constantsSEXP );
        Rcpp::traits::input_parameter< const IntegerVector >::type neurons_id(neurons_idSEXP );
        Rcpp::traits::input_parameter< const List >::type neurons_id_conn(neurons_id_connSEXP );
        Rcpp::traits::input_parameter< const List >::type neurons_w(neurons_wSEXP );
        Rcpp::traits::input_parameter< const List >::type net(netSEXP );
        SEXP __result = USRMs(t, constants, neurons_id, neurons_id_conn, neurons_w, net);
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}
// kernelPass_spikes
SEXP kernelPass_spikes(List d, const List kernel_options);
RcppExport SEXP snnSRM_kernelPass_spikes(SEXP dSEXP, SEXP kernel_optionsSEXP) {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        Rcpp::RNGScope __rngScope;
        Rcpp::traits::input_parameter< List >::type d(dSEXP );
        Rcpp::traits::input_parameter< const List >::type kernel_options(kernel_optionsSEXP );
        SEXP __result = kernelPass_spikes(d, kernel_options);
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}
// kernelPass_stat
SEXP kernelPass_stat(List d, const List kernel_options);
RcppExport SEXP snnSRM_kernelPass_stat(SEXP dSEXP, SEXP kernel_optionsSEXP) {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        Rcpp::RNGScope __rngScope;
        Rcpp::traits::input_parameter< List >::type d(dSEXP );
        Rcpp::traits::input_parameter< const List >::type kernel_options(kernel_optionsSEXP );
        SEXP __result = kernelPass_stat(d, kernel_options);
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}
// simLayers
SEXP simLayers(const List sim_options, const List constants, List layers, List net);
RcppExport SEXP snnSRM_simLayers(SEXP sim_optionsSEXP, SEXP constantsSEXP, SEXP layersSEXP, SEXP netSEXP) {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        Rcpp::RNGScope __rngScope;
        Rcpp::traits::input_parameter< const List >::type sim_options(sim_optionsSEXP );
        Rcpp::traits::input_parameter< const List >::type constants(constantsSEXP );
        Rcpp::traits::input_parameter< List >::type layers(layersSEXP );
        Rcpp::traits::input_parameter< List >::type net(netSEXP );
        SEXP __result = simLayers(sim_options, constants, layers, net);
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}
// timesTwo
SEXP timesTwo(NumericVector x);
RcppExport SEXP snnSRM_timesTwo(SEXP xSEXP) {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        Rcpp::RNGScope __rngScope;
        Rcpp::traits::input_parameter< NumericVector >::type x(xSEXP );
        SEXP __result = timesTwo(x);
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}
