// This file was generated by Rcpp::compileAttributes
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// probFun
double probFun(double u, const List c);
RcppExport SEXP snn_probFun(SEXP uSEXP, SEXP cSEXP) {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        Rcpp::RNGScope __rngScope;
        Rcpp::traits::input_parameter< double >::type u(uSEXP );
        Rcpp::traits::input_parameter< const List >::type c(cSEXP );
        double __result = probFun(u, c);
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}
// kernelWindow
SEXP kernelWindow(List d, const List kernel_options);
RcppExport SEXP snn_kernelWindow(SEXP dSEXP, SEXP kernel_optionsSEXP) {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        Rcpp::RNGScope __rngScope;
        Rcpp::traits::input_parameter< List >::type d(dSEXP );
        Rcpp::traits::input_parameter< const List >::type kernel_options(kernel_optionsSEXP );
        SEXP __result = kernelWindow(d, kernel_options);
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}
// kernelCorrMat
SEXP kernelCorrMat(List d, const List kernel_options);
RcppExport SEXP snn_kernelCorrMat(SEXP dSEXP, SEXP kernel_optionsSEXP) {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        Rcpp::RNGScope __rngScope;
        Rcpp::traits::input_parameter< List >::type d(dSEXP );
        Rcpp::traits::input_parameter< const List >::type kernel_options(kernel_optionsSEXP );
        SEXP __result = kernelCorrMat(d, kernel_options);
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}
// kernelCrossCorrMat
SEXP kernelCrossCorrMat(List d1, List d2, const List kernel_options);
RcppExport SEXP snn_kernelCrossCorrMat(SEXP d1SEXP, SEXP d2SEXP, SEXP kernel_optionsSEXP) {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        Rcpp::RNGScope __rngScope;
        Rcpp::traits::input_parameter< List >::type d1(d1SEXP );
        Rcpp::traits::input_parameter< List >::type d2(d2SEXP );
        Rcpp::traits::input_parameter< const List >::type kernel_options(kernel_optionsSEXP );
        SEXP __result = kernelCrossCorrMat(d1, d2, kernel_options);
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}
// kernelCrossCorr
SEXP kernelCrossCorr(List d1, List d2, const List kernel_options);
RcppExport SEXP snn_kernelCrossCorr(SEXP d1SEXP, SEXP d2SEXP, SEXP kernel_optionsSEXP) {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        Rcpp::RNGScope __rngScope;
        Rcpp::traits::input_parameter< List >::type d1(d1SEXP );
        Rcpp::traits::input_parameter< List >::type d2(d2SEXP );
        Rcpp::traits::input_parameter< const List >::type kernel_options(kernel_optionsSEXP );
        SEXP __result = kernelCrossCorr(d1, d2, kernel_options);
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}
// kernelCrossEntropy
SEXP kernelCrossEntropy(List d1, List d2, const List kernel_options);
RcppExport SEXP snn_kernelCrossEntropy(SEXP d1SEXP, SEXP d2SEXP, SEXP kernel_optionsSEXP) {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        Rcpp::RNGScope __rngScope;
        Rcpp::traits::input_parameter< List >::type d1(d1SEXP );
        Rcpp::traits::input_parameter< List >::type d2(d2SEXP );
        Rcpp::traits::input_parameter< const List >::type kernel_options(kernel_optionsSEXP );
        SEXP __result = kernelCrossEntropy(d1, d2, kernel_options);
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}
// decomposePatterns
SEXP decomposePatterns(List patt_net, NumericVector timeline, NumericVector labels);
RcppExport SEXP snn_decomposePatterns(SEXP patt_netSEXP, SEXP timelineSEXP, SEXP labelsSEXP) {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        Rcpp::RNGScope __rngScope;
        Rcpp::traits::input_parameter< List >::type patt_net(patt_netSEXP );
        Rcpp::traits::input_parameter< NumericVector >::type timeline(timelineSEXP );
        Rcpp::traits::input_parameter< NumericVector >::type labels(labelsSEXP );
        SEXP __result = decomposePatterns(patt_net, timeline, labels);
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}
// histKernel
SEXP histKernel(List d, const List kernel_options);
RcppExport SEXP snn_histKernel(SEXP dSEXP, SEXP kernel_optionsSEXP) {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        Rcpp::RNGScope __rngScope;
        Rcpp::traits::input_parameter< List >::type d(dSEXP );
        Rcpp::traits::input_parameter< const List >::type kernel_options(kernel_optionsSEXP );
        SEXP __result = histKernel(d, kernel_options);
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}
// kernelVanRossumDist
SEXP kernelVanRossumDist(List d1, List d2, const List kernel_options);
RcppExport SEXP snn_kernelVanRossumDist(SEXP d1SEXP, SEXP d2SEXP, SEXP kernel_optionsSEXP) {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        Rcpp::RNGScope __rngScope;
        Rcpp::traits::input_parameter< List >::type d1(d1SEXP );
        Rcpp::traits::input_parameter< List >::type d2(d2SEXP );
        Rcpp::traits::input_parameter< const List >::type kernel_options(kernel_optionsSEXP );
        SEXP __result = kernelVanRossumDist(d1, d2, kernel_options);
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}