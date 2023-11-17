#include <iomanip>
#include <iostream>
#include <vector>

std::vector<double>
generate(double (*fptr)(double x), double x0, double dx, int n) {
  double x = x0;
  std::vector<double> v(n);
  for (int i = 0; i < n; ++i) {
    v[i] = fptr(x);
    x += dx;
  }
  return v;
}

void tabulate(double (*fptr)(double x), double a, double b, int n) {
  if (a > b) std::swap(a, b);
  const double dx = (b - a) / (n - 1);
  const auto v = generate(fptr, a, dx, n);
  
  double x = a;
  for (int i = 0; i < n; ++i) {
    std::cout << std::setw(10) << x << std::setw(10) << v[i] << '\n';
    x += dx; 
  }
}

double f(double x) { return cos(x) + sin(x); }

int main() {
  tabulate(f, 0, 5, 10);
  return 0;
}
