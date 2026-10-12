#include <iostream>
#include <iomanip>
#include <windows.h>
using namespace std;

struct ZScoreResult {
    double mean;
    double stdDev;
    int status;
};

struct IQRBounds {
    double q1;
    double q3;
    double iqr;
    double lowerBound;
    double upperBound;
    int status;
};

typedef ZScoreResult (*ZScoreFunc)(const double*, int, double*);
typedef IQRBounds    (*IqrOutliersFunc)(const double*, int);
typedef int          (*DetectOutliersFunc)(const double*, int, double, int*);
typedef double       (*AnomalyRateFunc)(const double*, int, double);

int main() {
    HMODULE hLib = LoadLibrary(TEXT("AnomalyDetection.dll"));
    if (!hLib) {
        cerr << " cannot load AnomalyDetection.dll\n";
        return 1;
    }

    auto zScore         = reinterpret_cast<ZScoreFunc>(GetProcAddress(hLib, "zScore"));
    auto iqrOutliers    = reinterpret_cast<IqrOutliersFunc>(GetProcAddress(hLib, "iqrOutliers"));
    auto detectOutliers = reinterpret_cast<DetectOutliersFunc>(GetProcAddress(hLib, "detectOutliers"));
    auto anomalyRate    = reinterpret_cast<AnomalyRateFunc>(GetProcAddress(hLib, "anomalyRate"));

    if (!zScore || !iqrOutliers || !detectOutliers || !anomalyRate) {
        cerr << "ERROR: functions not found in DLL\n";
        FreeLibrary(hLib);
        return 1;
    }

    double data[] = {1.0, 2.0, 2.1, 1.9, 2.0, 50.0, 2.2, 1.8};
    int n = 8;

    cout << fixed << setprecision(4);

    double zScores[8];
    ZScoreResult zr = zScore(data, n, zScores);
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

    FreeLibrary(hLib);
    cout << "\nDLL unloaded.\n";
    return 0;
}