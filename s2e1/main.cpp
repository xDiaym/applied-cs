#include <iostream>
#include <cmath>
#include <math.h>
#include <ostream>

class Complex {
    float im, re;

public:
    Complex(float re_ = 0, float im_ = 0)
        : im(im_), re(re_) {}

    static Complex fromAngle(float angle, float R) {
        return {std::cos(angle) * R, std::sin(angle) * R};
    }

    float Re() const noexcept {
        return re;
    }

    float Im() const noexcept {
        return im;
    }

    Complex operator+(Complex other) {
        return {re + other.re, im + other.im};
    }

    Complex operator-(Complex other) {
        return {re - other.re, im - other.im};
    }

    Complex operator*(Complex other) {
        return {re*other.re - im*other.im, re*other.im + im*other.re};
    }

    Complex operator/(Complex other) {
        const float r = other.re*other.re + other.im*other.im;
        return {(re*other.re + im*other.im)/r, (other.re*im - re*other.im)/r};
    }
};

std::ostream& operator<<(std::ostream& os, Complex c) {
    return (os << c.Re() << " + " << c.Im() << "*i");
}

int main() {
    Complex a(0, 1), b = Complex::fromAngle(3.1415926, 1);
    // Complex a(1, 2), b(3, 4);
    std::cout << a + b << std::endl;
    std::cout << a - b << std::endl;
    std::cout << a * b << std::endl;
    std::cout << a / b << std::endl;
}