/*
 *
 */

#include "catch.hpp"
#include "json.hpp"

using json = nlohmann::json;

// Возвращает значение pi
double pi();

// Возвращает значение Г-функции для целого индекса
int factorial(int k);

// Возвращает значение Г-функции для полуцелого индекса
double gamma(double k);

namespace filesys {
    // Считать значения из файла
    json readFile(const std::string& filename);
    // Вывод значений в файл
    void writeFile(const std::string& filename, const json& data);
}
