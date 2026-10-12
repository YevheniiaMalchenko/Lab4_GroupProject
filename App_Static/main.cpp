#include <iostream>
#include <vector>
#include <iomanip>
#include "AnomalyDetection.h"

int main() {
    std::cout << "====================================================\n";
    std::cout << "  App_Static: Testing AnomalyDetection (STATIC LIB) \n";
    std::cout << "====================================================\n\n";

    // 1. Тестові дані з аномаліями
    std::vector<double> dataset = {10.2, 10.5, 9.8, 10.1, 10.3, 9.9, 10.0, 45.0, 10.4, -25.0};
    int n = static_cast<int>(dataset.size());

    std::cout << "Input dataset (" << n << " items):\n";
    for (int i = 0; i < n; ++i) {
        std::cout << dataset[i] << " ";
    }
    std::cout << "\n\n";

    // 2. Тестування zScore()
    std::vector<double> zScores(n);
    ZScoreResult zRes = zScore(dataset.data(), n, zScores.data());

    std::cout << "[1] Testing zScore():\n";
    std::cout << "    Status: " << zRes.status << "\n";
    std::cout << "    Mean: " << std::fixed << std::setprecision(3) << zRes.mean << "\n";
    std::cout << "    StdDev: " << zRes.stdDev << "\n";
    std::cout << "    Calculated Z-Scores:\n    ";
    for (int i = 0; i < n; ++i) {
        std::cout << zScores[i] << " ";
    }
    std::cout << "\n\n";

    // 3. Тестування iqrOutliers()
    IQRBounds iqrRes = iqrOutliers(dataset.data(), n);
    std::cout << "[2] Testing iqrOutliers():\n";
    std::cout << "    Status: " << iqrRes.status << "\n";
    std::cout << "    Q1 (25%): " << iqrRes.q1 << "\n";
    std::cout << "    Q3 (75%): " << iqrRes.q3 << "\n";
    std::cout << "    IQR: " << iqrRes.iqr << "\n";
    std::cout << "    Tukey Bounds: [" << iqrRes.lowerBound << ", " << iqrRes.upperBound << "]\n\n";

    // 4. Тестування detectOutliers() (поріг threshold = 2.0)
    double threshold = 2.0;
    std::vector<int> flags(n);
    int outlierCount = detectOutliers(dataset.data(), n, threshold, flags.data());

    std::cout << "[3] Testing detectOutliers(threshold = " << threshold << "):\n";
    std::cout << "    Detected outliers count: " << outlierCount << "\n";
    for (int i = 0; i < n; ++i) {
        std::cout << "    Item [" << i << "] = " << std::setw(6) << dataset[i] 
                  << " -> " << (flags[i] ? "ANOMALY (OUTLIER)!" : "Normal") << "\n";
    }
    std::cout << "\n";

    // 5. Тестування anomalyRate()
    double rate = anomalyRate(dataset.data(), n, threshold);
    std::cout << "[4] Testing anomalyRate():\n";
    std::cout << "    Anomaly rate: " << (rate * 100.0) << "%\n\n";

    std::cout << "App_Static finished successfully!\n";
    return 0;
}
