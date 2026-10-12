#pragma once

#ifdef ANOMALY_EXPORTS
    #define ANOMALY_API extern "C" __declspec(dllexport)
#elif defined(ANOMALY_IMPORTS)
    #define ANOMALY_API extern "C" __declspec(dllimport)
#else
    #define ANOMALY_API extern "C"
#endif

/**
 * @brief Структура для зберігання результату обчислення Z-Score.
 */
struct ZScoreResult {
    double mean;       ///< Середнє арифметичне вибірки
    double stdDev;     ///< Стандартне відхилення вибірки
    int status;        ///< 0 - успіх, -1 - помилка (nullptr, недостатньо даних), -2 - нульова дисперсія
};

/**
 * @brief Структура для збереження меж IQR.
 */
struct IQRBounds {
    double q1;         ///< Перший квартиль (25%)
    double q3;         ///< Третій квартиль (75%)
    double iqr;        ///< Інтерквартильний розмах (q3 - q1)
    double lowerBound; ///< Нижня межа (q1 - 1.5 * iqr)
    double upperBound; ///< Верхня межа (q3 + 1.5 * iqr)
    int status;        ///< 0 - успіх, -1 - помилка
};

/**
 * @brief Обчислює Z-Score для кожного елемента вхідного масиву.
 * 
 * @param data Вхідний масив чисел.
 * @param n Розмір масиву (повинен бути >= 2).
 * @param outZScores Вихідний буфер розміру n для збереження z-значень.
 * @return ZScoreResult Структура з mean, stdDev та кодом статусу.
 */
ANOMALY_API ZScoreResult zScore(const double* data, int n, double* outZScores);

/**
 * @brief Обчислює квартилі та межі Викидів за методом IQR.
 * 
 * @param data Вхідний масив чисел.
 * @param n Розмір масиву (повинен бути >= 4).
 * @return IQRBounds Структура з розрахованими межами або помилкою.
 */
ANOMALY_API IQRBounds iqrOutliers(const double* data, int n);

/**
 * @brief Виявляє аномалії у масиві на основі порогу Z-Score (|z| > threshold).
 * 
 * @param data Вхідний масив чисел.
 * @param n Розмір масиву.
 * @param threshold Поріг z-оцінки (зазвичай 2.0 або 3.0, повинен бути > 0).
 * @param outAnomalyFlags Вихідний буфер цілих чисел (0 - норма, 1 - аномалія).
 * @return int Кількість знайдених аномалій, або -1 при некоректних даних.
 */
ANOMALY_API int detectOutliers(const double* data, int n, double threshold, int* outAnomalyFlags);

/**
 * @brief Обчислює відсоток (частку) аномалій у вибірці від 0.0 до 1.0.
 * 
 * @param data Вхідний масив чисел.
 * @param n Розмір масиву.
 * @param threshold Поріг z-оцінки.
 * @return double Частка аномалій (0.0..1.0), або -1.0 при помилці.
 */
ANOMALY_API double anomalyRate(const double* data, int n, double threshold);
