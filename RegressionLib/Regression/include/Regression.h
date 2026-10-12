#pragma once
#ifdef REGRESSION_EXPORTS
#define REGRESSION_API extern "C" __declspec(dllexport)
#else
#define REGRESSION_API extern "C" __declspec(dllimport)
#endif

struct RegressionResult {
    double a;
    double b;
};

REGRESSION_API RegressionResult linearRegression(const double* x, const double* y, int n);
REGRESSION_API double predict(const RegressionResult& model, double x);
REGRESSION_API double rSquared(const double* x, const double* y, int n, const RegressionResult& model);
REGRESSION_API int residuals(const double* x, const double* y, int n, const RegressionResult& model, double* outResiduals);