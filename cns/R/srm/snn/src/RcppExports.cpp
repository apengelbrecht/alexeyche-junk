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
