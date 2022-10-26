#include "vector2d.h"
#include <cmath>

Vector2d::Vector2d(double i, double j)
{
    x = i;
    y = j;
}

Vector2d& Vector2d::add(const Vector2d& other) {
    this->x += other.x;
    this->y += other.y;
    return *this;
}

Vector2d& Vector2d::subtract(const Vector2d& other) {
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}

Vector2d& Vector2d::abs() {
    this->x = std::abs(this->x);
    this->y = std::abs(this->y);
    return *this;
}

Vector2d& Vector2d::multiply(const int& other) {
    this->x *= other;
    this->y *= other;
    return *this;
}

Vector2d& Vector2d::divide(const int& other) {
    this->x /= other;
    this->y /= other;
    return *this;
}

Vector2d& Vector2d::operator+(const Vector2d& RHS) {
    return this->add(RHS);
}

Vector2d& Vector2d::operator-(const Vector2d& RHS) {
    return this->subtract(RHS);
}

Vector2d& Vector2d::operator*(const double& RHS) {
    return this->multiply(RHS);
}

Vector2d& Vector2d::operator/(const double& RHS) {
    return this->divide(RHS);
}