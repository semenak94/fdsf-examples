/**
 * Интегральная ФФД
 */

#include "Common.h"
#include <iostream>

// Константа Эйлера
inline const double eilerConst() {
    return 0.5772156649015325;
}

// Константа j
inline const double j() {
    return 0.76740941382814898;
}

namespace {
    // Путь до ресурсов
    const std::string RESOURCE = "../../resource/fully-conv-series/";

    /**
    * Класс расчета ряда log(n)/(n^2), необходим для вычисления коээффициентов при x>>1
    */
    class SeriesLogNDivSqrN {
    public:
        SeriesLogNDivSqrN()
            : m_seriesSum(sum()) {}
        SeriesLogNDivSqrN(size_t optimal_N)
            : m_N(optimal_N)
            , m_seriesSum(sum()) {}

        // Получение значения
        double get() const {
            return m_seriesSum;
        }
        // Число членов, по которому обрезают сумму
        size_t upperBound() const {
            return m_N;
        }
        // Рассчет суммы 2<=n<=N-1
        double limitedSum() const {
            size_t N = upperBound();
            double sumValue = log(N) / (2 * pow(N, 2));
            for (size_t n = N - 1; n > 1; --n) {
                sumValue += static_cast<double>(log(n)) / pow(n, 2);
            }
            return sumValue;
        }

    private:
        // Оптимальное N
        size_t m_N{ 300 };
        // Значение ряда
        double m_seriesSum{ 0 };

        // Вычисление суммы ряда
        double sum() const {
            return limitedSum() + integralPart() + first() + second();
        }
        // Расчет первой поправки
        double first() const {
            size_t N = upperBound();
            return (2 * log(N) - 1) / (12 * pow(N, 3));
        }
        // Расчет второй поправки
        double second() const {
            size_t N = upperBound();
            return (26 - 24 * log(N)) / (720.0 * pow(N, 5));
        }
        // Интегральная часть
        double integralPart() const {
            size_t N = upperBound();
            return (1 + log(N)) / N;
        }
    };


    // Расчет коэффициентов при x<0
    std::vector<double> calculateAk(size_t N) {
        std::vector<double> a;
        for (size_t n = 2; n < N; ++n) {
            double value = 0;
            for (size_t p = 1; p < n; ++p) {
                value += pow(p*(n-p), -0.5);
            }
            a.push_back(value/n);
        }
        return a;
    }

    // Всюду сходящийся ряд
    std::vector<double> calculateCk(size_t N) {
        std::vector<double> ck = { 1.0 / 2 };
        std::vector<double> bm12 = filesys::readFile(RESOURCE + "bm12.json");
        for (size_t n = 1; n < N; ++n) {
            double sum = 0;
            for (size_t p = 0; p <= n; ++p) {
                sum += bm12.at(p)*bm12.at(n - p);
            }
            double result = ((n + 1)*ck.at(n - 1) + sum) / (n + 2);
            ck.push_back(result);
        }
        return ck;
    }

    // Вычисление константы j
    inline const double calculateConstj() {
        SeriesLogNDivSqrN example;
        double value = (1 - 2 * log(2) / 3 - eilerConst() / 3);
        double jConst = pow(pi(), 2)*value / 2 - example.get();
        return jConst;
    }

    // Вычисление Ck x-> +inf
    std::vector<double> calculateCRight(int N) {
        double k = -0.5;
        double prod = 1, kPairsProd = (k + 1);
        std::vector<double> result = { 1 }, A = { 1 };//, -pi()*pi() / 24 };
        for (int q = 1; q <= N; ++q) {
            double mul = 2 * (1 - pow(2, 1 - 2 * q));
            prod *= kPairsProd*(kPairsProd - 1);
            kPairsProd -= 2;
            A.push_back(mul*dzetaFunction(2*q)*prod);
        }
        for (int n = 1; n <= N; ++n) {
            double sum = 0;
            for (int q = 0; q <= n; ++q) {
                sum += A.at(q)*A.at(n - q);
            }
            result.push_back(sum);
        }
        return result;
    }
}


TEST_CASE("seriesCheck") {
    SECTION("convergence") {
        std::cout << "N=100: " << SeriesLogNDivSqrN(100).get() << std::endl;
        std::cout << "N=200: " << SeriesLogNDivSqrN(200).get() << std::endl;
        std::cout << "N=300: " << SeriesLogNDivSqrN().get() << std::endl;
        std::cout << "N=10^5 " << SeriesLogNDivSqrN(100000).get() << std::endl;
    }
}

TEST_CASE("iffd") {
    // x<0
    SECTION("left") {
        std::vector<double> a = calculateAk(34);
        filesys::writeFile("exp-c.json", a);
    }
    SECTION("conv") {
        std::vector<double> C = calculateCk(67);
        filesys::writeFile("fcs-c.json", C);
    }
    // x>0
    SECTION("right") {
        std::vector<double> coeff = calculateCRight(12);
        filesys::writeFile("asympt-c.json", coeff);
    }
}
