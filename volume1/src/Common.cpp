#include "Common.h"

#include <map>
#include <cmath>
#include <fstream>

// Возвращает значение pi
double pi() {
    return 4*atan(1);
}

// Возвращает значение Г-функции для целого индекса
int factorial(int k) {
    if (k < 0) {
        throw std::invalid_argument("k must be greater or equal 0");
    }
    return k == 0 ? 1 : k*factorial(k - 1);
}

// Возвращает значение Г-функции для полуцелого индекса
double gamma(double k) {
    std::map<double, double> SUPPORTED_INDICES = {
        { -1.5, -2 * sqrt(pi()) },
        { -0.5, sqrt(pi()) },
        { 0.5, sqrt(pi()) / 2 },
        { 1.5, 3 * sqrt(pi()) / 4 },
        { 2.5, 15 * sqrt(pi()) / 8 },
        { 3.5, 105 * sqrt(pi()) / 16 }
    };

    return SUPPORTED_INDICES.at(k);
}

// Нечетное число
bool isOdd(int n) {
    return n % 2 != 0;
}

// Дзета-функция
double dzetaFunction(int n) {
    if (isOdd(n)) {
        return 0;
    }
    if (n > 24) {
        throw std::domain_error("n is too large");
    }
    const std::array<double, 12> DZETA = {
        1.0 / 6,
        1.0 / 90,
        1.0 / 945,
        1.0 / 9450,
        1.0 / 93550,
        691.0 / 638512875,
        2.0 / 18243225,
        3617.0 / 325641566250,
        43867.0 / 38979295480125,
        174611.0 / 1531329465290625,
        77683.0 / 13447856940643124,
        236364091.0 / 201919571963756511232.0
    };

    return DZETA.at(n / 2 - 1)*pow(pi(), n);
}

namespace filesys {

    // Считать значения из файла
    json readFile(const std::string& filename) {
        std::ifstream file(filename);
        json jsonData;
        file >> jsonData;
        file.close();
        return jsonData;
    }

    // Вывод значений в файл
    void writeFile(const std::string& filename, const json& data) {
        std::ofstream file(filename);
        file.precision(std::numeric_limits<double>::max_digits10);
        file << data << std::endl;
        file.close();
    }
}