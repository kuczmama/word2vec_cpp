#ifndef VECTOR_H
#define VECTOR_H

#include <vector>
#include <functional>
#include <numeric>
#include <cmath>

class Vector {
private:
    std::vector<double> data;

public:
    Vector();  // Default constructor
    Vector(int size, const std::vector<double>& initial_data = {});

    double& operator[](size_t index);
    const double& operator[](size_t index) const;
    size_t size() const;
    double dot(const Vector& other) const;
    double norm() const;
    double cosine_similarity(const Vector& other) const;

    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    Vector operator*(const Vector& other) const;
    Vector operator/(const Vector& other) const;
    Vector& operator+=(const Vector& other);
    Vector operator*(double scalar) const;

private:
    Vector binary_operation(const Vector& other, std::function<double(double, double)> op) const;
};

#endif // VECTOR_H
