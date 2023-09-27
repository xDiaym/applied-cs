#include <iostream>
#include <iomanip>

void tabulate(double (*fptr)(double x), double a, double b, int n) {
    if (a > b) std::swap(a, b);
    double dx = (b - a) / (n - 1);
    double x = a;
    for (int i = 0; i < n; ++i) {
        std::cout << std::setw(10) << x << '\t' << fptr(x) << '\n';
        x += dx;
    }
}

double f(double x) { return cos(x) + sin(x); }

int main() {
    tabulate(f, 0, 3, 10);
}
