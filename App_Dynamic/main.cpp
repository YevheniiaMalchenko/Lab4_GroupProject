#include <iostream>
#include <vector>
#include <iomanip>
#include "AnomalyDetection.h"

int main() {
    std::cout << "=====================================================\n";
    std::cout << "  App_Dynamic: Testing AnomalyDetection (SHARED DLL) \n";
    std::cout << "               Implicit Linking via .lib             \n";
    std::cout << "=====================================================\n\n";

    // 1. Стандартний тестовий сценарій
    std::vector<double> sensorReadings = {15.0, 15.2, 14.9, 15.1, 100.0, 15.3, -50.0, 15.0};
    int n = static_cast<int>(sensorReadings.size());

    std::cout << "[1] Normal Data Test:\n    Values: ";
    for (double val : sensorReadings) std::cout << val << " ";
    std::cout << "\n";

    std::vector<double> zScores(n);
    ZScoreResult zRes = zScore(sensorReadings.data(), n, zScores.data());
    std::cout << "    Mean: " << zRes.mean << ", StdDev: " << zRes.stdDev << ", Status: " << zRes.status << "\n";

    IQRBounds bounds = iqrOutliers(sensorReadings.data(), n);
    std::cout << "    IQR Bounds: [" << bounds.lowerBound << ", " << bounds.upperBound << "]\n";

    double rate = anomalyRate(sensorReadings.data(), n, 2.0);
    std::cout << "    Anomaly Rate (threshold=2.0): " << rate * 100.0 << "%\n\n";

    // 2. Тестування Edge Cases (перевірка надійності DLL)
    std::cout << "[2] Edge Cases and Robustness Verification:\n";

    // А) Передача nullptr
    std::cout << "    a) Passing nullptr to zScore:\n";
    ZScoreResult nullRes = zScore(nullptr, 10, nullptr);
    std::cout << "       Result status: " << nullRes.status << " (Expected: -1) -> " 
              << (nullRes.status == -1 ? "PASS (No crash!)" : "FAIL") << "\n";

    // Б) Розмір масиву менше допустимого
    std::cout << "    b) Insufficient elements (n=1) to iqrOutliers:\n";
    double singleVal = 42.0;
    IQRBounds edgeIqr = iqrOutliers(&singleVal, 1);
    std::cout << "       Result status: " << edgeIqr.status << " (Expected: -1) -> "
              << (edgeIqr.status == -1 ? "PASS (No crash!)" : "FAIL") << "\n";

    // В) Усі елементи однакові (stdDev = 0, ділення на 0)
    std::cout << "    c) Constant data (zero variance / division by zero protection):\n";
    std::vector<double> constantData = {5.0, 5.0, 5.0, 5.0, 5.0};
    std::vector<double> constZ(5);
    ZScoreResult constRes = zScore(constantData.data(), 5, constZ.data());
    std::cout << "       Result status: " << constRes.status << " (Expected: -2) -> "
              << (constRes.status == -2 ? "PASS (Zero variance handled!)" : "FAIL") << "\n";

    // Г) Від'ємний або нульовий threshold
    std::cout << "    d) Invalid negative threshold (-1.5) to detectOutliers:\n";
    std::vector<int> flags(n);
    int invalidThresholdRes = detectOutliers(sensorReadings.data(), n, -1.5, flags.data());
    std::cout << "       Result: " << invalidThresholdRes << " (Expected: -1) -> "
              << (invalidThresholdRes == -1 ? "PASS (Rejected invalid parameter!)" : "FAIL") << "\n\n";

    std::cout << "App_Dynamic (DLL test) completed successfully!\n";
    return 0;
}
