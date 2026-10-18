#include <iostream>
#include <new>
#include <cstddef>
#include <limits>

using std::size_t;

void delete_matrix(int **matrix, size_t n) {
    if (matrix == nullptr)
        return;

    for (size_t i = 0; i < n; i++)
        delete[] matrix[i];
    delete[] matrix;
}

int **create_matrix(size_t n, size_t m) {
    size_t limit = std::numeric_limits<size_t>::max();
    if (n > limit / sizeof(int *) || m > limit / sizeof(int))
        return nullptr;

    int **matrix = nullptr;
    size_t created = 0;
    try {
        matrix = new int *[n];
        for (size_t i = 0; i < n; i++) {
            matrix[i] = new int[m];
            created++;
        }
    }
    catch (const std::bad_alloc &) {
        delete_matrix(matrix, created);
        return nullptr;
    }
    return matrix;
}

bool read_matrix(int **matrix, size_t n, size_t m) {
    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < m; j++)
            if (!(std::cin >> matrix[i][j]))
                return false;
    return true;
}

int **concat_rows(
    const int *const *lhs, size_t n1, size_t m1,
    const int *const *rhs, size_t n2, size_t m2) {
    size_t n = n1 > n2 ? n1 : n2;
    size_t limit = std::numeric_limits<size_t>::max();
    if (n > limit / sizeof(int *) || m1 > limit - m2)
        return nullptr;

    int **result = nullptr;
    size_t created = 0;
    try {
        result = new int *[n];
        for (size_t i = 0; i < n; i++) {
            size_t left = i < n1 ? m1 : 0;
            size_t right = i < n2 ? m2 : 0;
            if (left + right > limit / sizeof(int)) {
                delete_matrix(result, created);
                return nullptr;
            }
            result[i] = new int[left + right];
            created++;

            for (size_t j = 0; j < left; j++)
                result[i][j] = lhs[i][j];
            for (size_t j = 0; j < right; j++)
                result[i][left + j] = rhs[i][j];
        }
    }
    catch (const std::bad_alloc &) {
        delete_matrix(result, created);
        return nullptr;
    }
    return result;
}

int main() {
    int n1, m1, n2, m2;
    std::cout << "Enter n1 m1 n2 m2 (positive integers):\n";
    if (!(std::cin >> n1 >> m1 >> n2 >> m2) ||
        n1 <= 0 || m1 <= 0 || n2 <= 0 || m2 <= 0) {
        std::cerr << "Input error\n";
        return 1;
    }

    int **lhs = create_matrix(n1, m1);
    if (lhs == nullptr) {
        std::cerr << "Memory allocation error\n";
        return 2;
    }
    int **rhs = create_matrix(n2, m2);
    if (rhs == nullptr) {
        delete_matrix(lhs, n1);
        std::cerr << "Memory allocation error\n";
        return 2;
    }

    std::cout << "Enter the first matrix, then the second matrix:\n";
    if (!read_matrix(lhs, n1, m1) || !read_matrix(rhs, n2, m2)) {
        delete_matrix(lhs, n1);
        delete_matrix(rhs, n2);
        std::cerr << "Input error\n";
        return 1;
    }

    int **result = concat_rows(lhs, n1, m1, rhs, n2, m2);
    if (result == nullptr) {
        delete_matrix(lhs, n1);
        delete_matrix(rhs, n2);
        std::cerr << "Memory allocation error\n";
        return 2;
    }

    size_t n = n1 > n2 ? n1 : n2;
    std::cout << "Result:\n";
    for (size_t i = 0; i < n; i++) {
        size_t m = (i < static_cast<size_t>(n1) ? static_cast<size_t>(m1) : 0)
                 + (i < static_cast<size_t>(n2) ? static_cast<size_t>(m2) : 0);
        for (size_t j = 0; j < m; j++) {
            if (j > 0)
                std::cout << ' ';
            std::cout << result[i][j];
        }
        std::cout << '\n';
    }

    delete_matrix(lhs, n1);
    delete_matrix(rhs, n2);
    delete_matrix(result, n);
    return 0;
}
