#include <iostream>
#include <iomanip>

class Matrix {
    int m, n;
    int** mat;
public:
    Matrix(int n_, int m_) : m(m_), n(n_) {
        mat = new int*[n];
        for (int i = 0; i < n; ++i)
            mat[i] = new int[m];
    }

    void init() {
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                mat[i][j] = i*m+j;
    }

    void print() {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j)
                std::cout << std::setw(5) << mat[i][j];
            std::cout << '\n';
        }
    }

    ~Matrix() {
        for (int i = 0; i < n; ++i)
            delete[] mat[i];
        delete[] mat;
    }
};

int main() {
    int n, m; std::cin >> n >> m;
    Matrix mat(n, m);
    mat.init();
    mat.print();
    return 0;
}