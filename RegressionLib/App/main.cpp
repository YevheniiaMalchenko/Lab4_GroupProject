#include <iostream>
#include <iomanip>
#include "Regression.h"
using namespace std;

int main() {
    double x[] = {1, 2, 3, 4, 5};
    double y[] = {2.1, 3.9, 6.2, 7.8, 10.1};
    int n = 5;

    RegressionResult model = linearRegression(x, y, n);
    cout << fixed << setprecision(4);
    cout << "Model: y = " << model.a << " * x + " << model.b << "\n";
    cout << "Predict(6) = " << predict(model, 6) << "\n";
    cout << "R^2 = " << rSquared(x, y, n, model) << "\n";

    double res[5];
    if (residuals(x, y, n, model, res) == 0) {
        cout << "Residuals: ";
        for (double r : res) cout << r << " ";
        cout << "\n";
    }
    return 0;
}