#include "Regression.h"
#include <cmath>

RegressionResult linearRegression(const double* x, const double* y, int n) {
    RegressionResult result{0.0, 0.0};
    if (x == nullptr || y == nullptr || n < 2) return result;

    double sumX = 0.0, sumY = 0.0, sumXY = 0.0, sumX2 = 0.0;
    for (int i = 0; i < n; ++i) {
        sumX += x[i];
        sumY += y[i];
        sumXY += x[i] * y[i];
        sumX2 += x[i] * x[i];
    }

    double denom = n * sumX2 - sumX * sumX;
    if (std::fabs(denom) < 1e-12) return result;

    result.a = (n * sumXY - sumX * sumY) / denom;
    result.b = (sumY - result.a * sumX) / n;
    return result;
}

double predict(const RegressionResult& model, double x) {
    return model.a * x + model.b;
}

double rSquared(const double* x, const double* y, int n, const RegressionResult& model) {
    if (x == nullptr || y == nullptr || n < 2) return 0.0;

    double sumY = 0.0;
    for (int i = 0; i < n; ++i) sumY += y[i];
    double meanY = sumY / n;

    double ssTot = 0.0, ssRes = 0.0;
    for (int i = 0; i < n; ++i) {
        double pred = predict(model, x[i]);
        ssRes += (y[i] - pred) * (y[i] - pred);
        ssTot += (y[i] - meanY) * (y[i] - meanY);
    }

    if (ssTot < 1e-12) return 0.0;
    return 1.0 - ssRes / ssTot;
}

int residuals(const double* x, const double* y, int n, const RegressionResult& model, double* outResiduals) {
    if (x == nullptr || y == nullptr || outResiduals == nullptr || n < 1) return -1;
    for (int i = 0; i < n; ++i) {
        outResiduals[i] = y[i] - predict(model, x[i]);
    }
    return 0;
}