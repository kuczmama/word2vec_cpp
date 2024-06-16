#ifndef VECTOR_H
#define VECTOR_H

#include <vector>
#include <functional>

class Vector {
private:
    std::vector<double> data;

public:
    // Constructor
    Vector(int size, const std::vector<double>& initial_data = {});

    // Access and mutation methods
    double& operator[](size_t index);
    const double& operator[](size_t index) const;

    // Vector operations
    double dot(const Vector& other) const;
    double norm() const;
    double cosine_similarity(const Vector& other) const;

    // Arithmetic operators
    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    Vector operator*(const Vector& other) const;
    Vector operator/(const Vector& other) const;

private:
    Vector binary_operation(const Vector& other, std::function<double(double, double)> op) const;
};

#endif // VECTOR_H
