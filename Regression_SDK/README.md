# SDK бібліотеки Regression

Версія API: 1.0
Автор: Мальченко Євгенія, студент А


## Склад пакунка
- include/Regression.h — публічний заголовок.
- lib/Regression.lib — імпортна бібліотека (для неявного зв'язування).
- bin/Regression.dll — динамічна бібліотека (для запуску).

## API
RegressionResult linearRegression(const double* x, const double* y, int n);
double predict(const RegressionResult& model, double x);
double rSquared(const double* x, const double* y, int n, const RegressionResult& model);
int residuals(const double* x, const double* y, int n, const RegressionResult& model, double* outResiduals);


## Опис функцій
1. linearRegression(const double* x, const double* y, int n)
   Призначення: побудова лінійної регресії методом найменших квадратів.
   Параметри: x, y - масиви довжини n; n має бути не менше 2.
   Повертає: структуру RegressionResult з коефіцієнтами a та b.
   Обмеження: при x або y рівних NULL, або n < 2 - повертає {a=0, b=0}.
   При всіх однакових x - повертає {a=0, b=0}.

2. predict(const RegressionResult& model, double x)
   Призначення: прогноз значення y для заданого x.
   Параметри: model - структура з коефіцієнтами; x - число.
   Повертає: значення model.a * x + model.b.

3. rSquared(const double* x, const double* y, int n, const RegressionResult& model)
   Призначення: коефіцієнт детермінації R² (оцінка якості моделі).
   Параметри: x, y, n - як у linearRegression; model - побудована модель.
   Повертає: число від 0.0 до 1.0 (1.0 - ідеальна модель).
   Обмеження: при n < 2 або постійному y - повертає 0.0.

4. residuals(const double* x, const double* y, int n, const RegressionResult& model, double* outResiduals)

   Призначення: обчислення залишків residual_i = y_i - predict(x_i).
   Параметри: x, y, n, model - як вище; outResiduals - буфер довжини n.
   Повертає: 0 при успіху, -1 при помилці (NULL-вказівник або n < 1).
   Побічні ефекти: заповнює буфер outResiduals.


## Граничні випадки
x == NULL або y == NULL - linearRegression повертає {0, 0}
n < 2  - linearRegression повертає {0, 0}
Всі x однакові  - linearRegression повертає {0, 0}
Постійне y - rSquared повертає 0.0
outResiduals == NULL - residuals повертає -1
n < 1 - residuals повертає -1

