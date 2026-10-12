#include <iostream>
#include <iomanip>
#include "AnomalyDetection.h"
using namespace std;

int main() {
    double data[] = {1.0, 2.0, 2.1, 1.9, 2.0, 50.0, 2.2, 1.8};
    int n = 8;

    double zScores[8];
    ZScoreResult zr = zScore(data, n, zScores);

    cout << fixed << setprecision(4);
    cout << "Status: " << zr.status << "\n";
    if (zr.status == 0) {
        cout << "Mean: " << zr.mean << "\n";
        cout << "StdDev: " << zr.stdDev << "\n";
        cout << "Z-scores: ";
        for (int i = 0; i < n; ++i) cout << zScores[i] << " ";
        cout << "\n";
    }

    IQRBounds iqr = iqrOutliers(data, n);
    cout << "Status: " << iqr.status << "\n";
    if (iqr.status == 0) {
        cout << "Q1: " << iqr.q1 << "\n";
        cout << "Q3: " << iqr.q3 << "\n";
        cout << "IQR: " << iqr.iqr << "\n";
        cout << "Lower bound: " << iqr.lowerBound << "\n";
        cout << "Upper bound: " << iqr.upperBound << "\n";
    }

    int flags[8];
    int count = detectOutliers(data, n, 2.0, flags);
    cout << "Count of anomalies: " << count << "\n";
    if (count >= 0) {
        cout << "Flags: ";
        for (int i = 0; i < n; ++i) cout << flags[i] << " ";
        cout << "\n";
    }

    double rate = anomalyRate(data, n, 2.0);
    cout << "Anomaly rate: " << rate << "\n";

    return 0;
}