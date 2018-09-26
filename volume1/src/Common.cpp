#include "Common.h"

#include <map>
#include <cmath>
#include <fstream>

// Возвращает значение pi
double pi() {
    return 4*atan(1);
}

// Возвращает значение Г-функции для целого индекса
int gamma(int k) {
    if (k < 0) {
        throw std::invalid_argument("k must be greater or equal 0");
    }
    return k == 0 ? 1 : k*gamma(k - 1);
}

// Возвращает значение Г-функции для полуцелого индекса
double gamma(double k) {
    std::map<double, double> SUPPORTED_HALFINTEGER_INDICES = {
        { -1.5, -2 * sqrt(pi()) },
        { -0.5, sqrt(pi()) },
        { 0.5, sqrt(pi()) / 2 },
        { 1.5, 3 * sqrt(pi()) / 4 },
        { 2.5, 15 * sqrt(pi()) / 8 },
        { 3.5, 105 * sqrt(pi()) / 16 }
    };

    return SUPPORTED_HALFINTEGER_INDICES.at(k);
}
#include <iostream>
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