#include <iostream>
#include <iomanip>

bool isGood(int n) {
    int x = n;
    while (x > 0) {
        const int d = x % 10;
        if (d == 0 || n % d != 0)
            return false;
        x /= 10;
    }
    return true;
} 

int main() {
    int n;
    std::cin >> n;
    int w = 0;
    for (int i = 0; i < n; ++i) {
        if (isGood(i)) {
            std::cout << std::setw(8) << i;
            if (++w % 8 == 0)
                std::cout << '\n';
        }
    }
    return 0;
}
