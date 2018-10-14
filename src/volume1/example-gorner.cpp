/*
 * Схема Горнера для ряда x < 0.
 */

#include "Common.h"
#include <iostream>

namespace {

    // Заданная точность
    const double EPSILON = 10e-17;
    // Крайняя точка x, для которой требуется больше всего членов
    const double X_RIGHT = -1;

    bool isInteger(double n) {
        double intPart;
        return std::modf(n, &intPart) == 0.0;
    }

    // Оценить число членов ряда для схемы Горнера
    size_t memberQuantity(double x) {
        return static_cast<size_t>(ceil(log(EPSILON) / x));
    }

    // Вычислить функцию в точке x по схеме Горнера
    double gorner(double x, double k) {
        size_t N = memberQuantity(x);
        const double exp_x = exp(x);
        double sum = 1.0 / pow(N, k + 1);
        for (size_t i = N; i > 0; --i) {
            sum = 1 / pow(i, k + 1) - exp_x * sum;
        }
        auto gammaValue = isInteger(k) ? factorial(static_cast<int>(k)) : gamma(k);
        return sum * gammaValue * exp_x;
    }

    void checkGorner(double k) {
        double f;
        CHECK_NOTHROW(f = gorner(X_RIGHT, k));
        std::cout << "k = " << k << ", x = " << X_RIGHT << ", N = " << memberQuantity(X_RIGHT)
                  << ", I(k,x) = " << f << std::endl;
    }
}

TEST_CASE("gorner") {
    SECTION("khalf") {
        INFO("Вычисление по схеме Горнера функции k = 1/2 в точке x = -1");
        checkGorner(0.5);
    }
    SECTION("k1") {
        INFO("Вычисление по схеме Горнера функции k = 1 в точке x = -1");
        checkGorner(1);
    }
    SECTION("k3half") {
        INFO("Вычисление по схеме Горнера функции k = 3/2 в точке x = -1");
        checkGorner(1.5);
    }
    SECTION("k2") {
        INFO("Вычисление по схеме Горнера функции k = 2 в точке x = -1");
        checkGorner(2);
    }
    SECTION("k5half") {
        INFO("Вычисление по схеме Горнера функции k = 5/2 в точке x = -1");
        checkGorner(2.5);
    }
    SECTION("k3") {
        INFO("Вычисление по схеме Горнера функции k = 3 в точке x = -1")
        checkGorner(3);
    }
    SECTION("k7half") {
        INFO("Вычисление по схеме Горнера функции k = 7/2 в точке x = -1");
        checkGorner(3.5);
    }
    SECTION("k4") {
        INFO("Вычисление по схеме Горнера функции k = 4 в точке x = -1")
        checkGorner(4);
    }
}
