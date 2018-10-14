/**
 * Пример расчета коэффициентов для всюду сходящегося ряда (см. Препринт "Свойства функций" раздел 4)
 */

#include "Common.h"
#include <vector>
#include <iostream>
namespace {
    // Путь до ресурсов
    const std::string RESOURCE = "../../resource/fully-conv-series/";

    // Подынтегральная функция
    double f(size_t n, double tau) {
        double mul = pow(1 - 2*exp(-tau*tau), n);
        return mul*exp(-tau*tau);
    }

    // Формула трапеций
    double trapz(size_t n, size_t P) {
        double F = 0;
        double h = 8.0 / P;
        for (size_t p = P - 1; p > 0; --p) {
            F += f(n, 8*p/P);
        }
        return (2*F + pow(-1, n))*h;
    }

    /**
     * Вычисление коэффициентов b(n) для индекса k = -1/2 по методу,
     * близкому к Ричардсону( сверхстепенная сходимость )
     */
    double calculate(size_t N) {
        double k = -0.5;
        size_t P0 = 8;
        double I2n, In, stopCriteria;
        do {
            In = trapz(N, P0);
            I2n = trapz(N, 2 * P0);
            P0 *= 2;
            stopCriteria = abs(I2n - In);
        } while (stopCriteria > 1e-14);
        return I2n / gamma(k);
    }

    // Вычисление по рекуррентной формуле (12)
    std::vector<double> coefficients(const std::vector<double>& Bkm1) {
        std::vector<double> Bk;
        size_t N = Bkm1.size();
        double value = 0;
        for (size_t i = 0; i < N; ++i) {
            value += Bkm1.at(i);
            Bk.push_back(value / (i + 1));
        }
        return Bk;
    }

    // Рассчитать коэффициенты bk на основе коэффициентов bk-1
    void calculate(const std::string& source, const std::string& dest) {
        const std::string ext = ".json";
        std::vector<double> bkm1 = filesys::readFile(RESOURCE + source + ext);;
        std::vector<double> bk = coefficients(bkm1);
        filesys::writeFile(dest + ext, bk);
    }

    // Вычисление для индекса k = 0
    std::vector<double> calculateK0(size_t N) {
        std::vector<double> B0;
        for (size_t i = 0; i <= N; ++i) {
            double value = (1 + pow(-1.0, i)) / (2 * (i + 1));
            B0.push_back(value);
        }
        return B0;
    }
}

TEST_CASE("bm12") {
    INFO("Расчет коэффициентов всюду сходящегося ряда для индекса k = -1/2");
    std::vector<double> bm12;
    constexpr size_t N = 67;
    for (size_t n = 0; n <= N; ++n) {
        bm12.push_back(calculate(n));
    }
    json j = json::array();
    j = bm12;
    filesys::writeFile("bm12.json", j);
}

TEST_CASE("integer") {
    SECTION("k0") {
        INFO("Расчет коэффициентов индекса k = 0 по формуле (13)");
        size_t N = 67;
        std::vector<double> b0 = calculateK0(N);
        json j = json::array();
        j = b0;
        filesys::writeFile("b0.json", j);
    }
    SECTION("k1") {
        INFO("Расчет коэффициентов индекса k = 1 по формуле (12)");
        calculate("b0", "b1");
    }
    SECTION("k2") {
        INFO("Расчет коэффициентов индекса k = 2 по формуле (12)");
        calculate("b1", "b2");
    }
    SECTION("k3") {
        INFO("Расчет коэффициентов индекса k = 3 по формуле (12)");
        calculate("b2", "b3");
    }
    SECTION("k4") {
        INFO("Расчет коэффициентов индекса k = 4 по формуле (12)");
        calculate("b3", "b4");
    }
}

TEST_CASE("half") {
    SECTION("0.5") {
        INFO("Расчет коэффициентов индекса k = 1/2 по формуле (12)");
        calculate("bm12","b12");
    }
    SECTION("1.5") {
        INFO("Расчет коэффициентов индекса k = 3/2 по формуле (12)");
        calculate("b12", "b32");
    }
    SECTION("2.5") {
        INFO("Расчет коэффициентов индекса k = 5/2 по формуле (12)");
        calculate("b32", "b52");
    }
    SECTION("3.5") {
        INFO("Расчет коэффициентов индекса k = 7/2 по формуле (12)");
        calculate("b52", "b72");
    }
}
