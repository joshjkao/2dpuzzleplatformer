#ifndef VECTOR_2D
#define VECTOR_2D

struct Vector2d {
    double x;
    double y;

    Vector2d(double i = 0, double j = 0);

    Vector2d& add(const Vector2d& other);
    Vector2d& subtract(const Vector2d& other);
    Vector2d& abs();

    Vector2d& multiply(const int& other);
    Vector2d& divide(const int& other);

    Vector2d& operator+(const Vector2d& RHS);
    Vector2d& operator-(const Vector2d& RHS);

    Vector2d& operator*(const double& RHS);
    Vector2d& operator/(const double& RHS);

};

#endif