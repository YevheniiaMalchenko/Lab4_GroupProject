#include "AnomalyDetection.h"
#include <cmath>
#include <vector>
#include <algorithm>

namespace {
    // Допоміжна функція для обчислення перцентилю/квартилю методом лінійної інтерполяції
    double calculatePercentile(const std::vector<double>& sortedData, double p) {
        if (sortedData.empty()) return 0.0;
        if (sortedData.size() == 1) return sortedData[0];

        double rank = p * (sortedData.size() - 1);
        size_t low = static_cast<size_t>(std::floor(rank));
        size_t high = static_cast<size_t>(std::ceil(rank));
        double weight = rank - low;

        return sortedData[low] * (1.0 - weight) + sortedData[high] * weight;
    }
}

extern "C" {

ZScoreResult zScore(const double* data, int n, double* outZScores) {
    ZScoreResult res = {0.0, 0.0, -1};

    if (data == nullptr || outZScores == nullptr || n < 2) {
        return res;
    }

    // 1. Обчислення середнього
    double sum = 0.0;
    for (int i = 0; i < n; ++i) {
        sum += data[i];
    }
    double mean = sum / n;

    // 2. Обчислення дисперсії
    double sumSqDiff = 0.0;
    for (int i = 0; i < n; ++i) {
        double diff = data[i] - mean;
        sumSqDiff += diff * diff;
    }

    double variance = sumSqDiff / (n - 1); // незміщена вибіркова дисперсія
    double stdDev = std::sqrt(variance);

    res.mean = mean;
    res.stdDev = stdDev;

    // Перевірка на нульове або майже нульове відхилення
    if (stdDev < 1e-12) {
        for (int i = 0; i < n; ++i) {
            outZScores[i] = 0.0;
        }
        res.status = -2; // всі значення однакові
        return res;
    }

    // 3. Обчислення Z-Score для кожного елемента
    for (int i = 0; i < n; ++i) {
        outZScores[i] = (data[i] - mean) / stdDev;
    }

    res.status = 0;
    return res;
}

IQRBounds iqrOutliers(const double* data, int n) {
    IQRBounds bounds = {0.0, 0.0, 0.0, 0.0, 0.0, -1};

    if (data == nullptr || n < 4) {
        return bounds;
    }

    std::vector<double> sortedData(data, data + n);
    std::sort(sortedData.begin(), sortedData.end());

    double q1 = calculatePercentile(sortedData, 0.25);
    double q3 = calculatePercentile(sortedData, 0.75);
    double iqr = q3 - q1;

    bounds.q1 = q1;
    bounds.q3 = q3;
    bounds.iqr = iqr;
    bounds.lowerBound = q1 - 1.5 * iqr;
    bounds.upperBound = q3 + 1.5 * iqr;
    bounds.status = 0;

    return bounds;
}

int detectOutliers(const double* data, int n, double threshold, int* outAnomalyFlags) {
    if (data == nullptr || outAnomalyFlags == nullptr || n < 2 || threshold <= 0.0) {
        return -1;
    }

    std::vector<double> zScores(n);
    ZScoreResult zRes = zScore(data, n, zScores.data());

    if (zRes.status == -1) {
        return -1;
    }

    // Якщо відхилення нульове (всі числа однакові), аномалій немає
    if (zRes.status == -2) {
        for (int i = 0; i < n; ++i) {
            outAnomalyFlags[i] = 0;
        }
        return 0;
    }

    int anomalyCount = 0;
    for (int i = 0; i < n; ++i) {
        if (std::abs(zScores[i]) > threshold) {
            outAnomalyFlags[i] = 1;
            anomalyCount++;
        } else {
            outAnomalyFlags[i] = 0;
        }
    }

    return anomalyCount;
}

double anomalyRate(const double* data, int n, double threshold) {
    if (data == nullptr || n < 2 || threshold <= 0.0) {
        return -1.0;
    }

    std::vector<int> flags(n);
    int anomalies = detectOutliers(data, n, threshold, flags.data());

    if (anomalies < 0) {
        return -1.0;
    }

    return static_cast<double>(anomalies) / static_cast<double>(n);
}

} // extern "C"
