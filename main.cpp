#include <iostream>
#include <cstddef>
#include <limits>
#include <new>

int main() {
    long long input_rows = 0;
    long long input_cols = 0;
    if (!(std::cin >> input_rows >> input_cols) ||
        input_rows <= 0 || input_cols <= 0) {
        std::cerr << "Ошибка ввода размеров матрицы\n";
        return 1;
    }

    std::size_t rows = static_cast<std::size_t>(input_rows);
    std::size_t cols = static_cast<std::size_t>(input_cols);
    if (rows > std::numeric_limits<std::size_t>::max() / sizeof(int*) ||
        cols > std::numeric_limits<std::size_t>::max() / sizeof(int)) {
        std::cerr << "Не удалось выделить память\n";
        return 2;
    }

    int** matrix = nullptr;
    std::size_t created = 0;

    try {
        matrix = new int*[rows];
        for (std::size_t i = 0; i < rows; i++) {
            matrix[i] = new int[cols];
            created++;
        }
    } catch (const std::bad_alloc&) {
        for (std::size_t i = 0; i < created; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;
        std::cerr << "Не удалось выделить память\n";
        return 2;
    }

    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            if (!(std::cin >> matrix[i][j])) {
                std::cerr << "Ошибка ввода матрицы\n";
                for (std::size_t k = 0; k < rows; k++) {
                    delete[] matrix[k];
                }
                delete[] matrix;
                return 1;
            }
        }
    }
    for (std::size_t j = 0; j < cols; j++) {
        for (std::size_t i = 0; i < rows; i++) {
            if (i != 0) {
                std::cout << ' ';
            }
            std::cout << matrix[i][j];
        }
        std::cout << '\n';
    }
    for (std::size_t i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;

    return 0;
}

