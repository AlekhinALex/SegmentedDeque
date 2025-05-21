#pragma once
#include <iostream>
#include <cmath>

class Complex
{
private:
    double real;
    double imag;

public:
    Complex() : real(0.0), imag(0.0) {}
    Complex(double r) : real(r), imag(0.0) {}
    Complex(double r, double i) : real(r), imag(i) {}

    double getReal() const { return real; }
    double getImag() const { return imag; }

    void setReal(double r) { real = r; }
    void setImag(double i) { imag = i; }

    double magnitude() const
    {
        return std::sqrt(real * real + imag * imag);
    }

    Complex operator+(const Complex &other) const
    {
        return Complex(real + other.real, imag + other.imag);
    }

    Complex operator-(const Complex &other) const
    {
        return Complex(real - other.real, imag - other.imag);
    }

    Complex operator*(const Complex &other) const
    {
        return Complex(
            real * other.real - imag * other.imag,
            real * other.imag + imag * other.real);
    }

    bool operator==(const Complex &other) const
    {
        return real == other.real && imag == other.imag;
    }

    bool operator!=(const Complex &other) const
    {
        return !(*this == other);
    }

    bool operator<(const Complex &other) const
    {
        return magnitude() < other.magnitude();
    }

    bool operator>(const Complex &other) const
    {
        return magnitude() > other.magnitude();
    }
};

inline std::ostream &operator<<(std::ostream &os, const Complex &c)
{
    if (c.getImag() >= 0)
    {
        os << c.getReal() << "+" << c.getImag() << "i";
    }
    else
    {
        os << c.getReal() << c.getImag() << "i";
    }
    return os;
}
