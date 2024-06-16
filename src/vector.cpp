#include "vector.h"
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <numeric>

Vector::Vector(int size, const std::vector<double>& initial_data) {
    if (!initial_data.empty()) {
        data = initial_data;
    } else {
        data.resize(size);
        std::generate(data.begin(), data.end(), []() { return (rand() / (double)RAND_MAX) - 0.5; });
    }
}

double& Vector::operator[](size_t index) {
    if (index >= data.size()) throw std::out_of_range("Index out of range");
    return data[index];
}

const double& Vector::operator[](size_t index) const {
    if (index >= data.size()) throw std::out_of_range("Index out of range");
    return data[index];
}

double Vector::dot(const Vector& other) const {
    if (data.size() != other.data.size()) {
        throw std::invalid_argument("Vectors must have the same length");
    }
    return std::inner_product(data.begin(), data.end(), other.data.begin(), 0.0);
}

double Vector::norm() const {
    return std::sqrt(std::inner_product(data.begin(), data.end(), data.begin(), 0.0));
}

double Vector::cosine_similarity(const Vector& other) const {
    double denom = norm() * other.norm();
    if (denom == 0) throw std::runtime_error("Norm is zero, division by zero encountered");
    return dot(other) / denom;
}

Vector Vector::operator+(const Vector& other) const {
    return binary_operation(other, std::plus<double>());
}

Vector Vector::operator-(const Vector& other) const {
    return binary_operation(other, std::minus<double>());
}

Vector Vector::operator*(const Vector& other) const {
    return binary_operation(other, std::multiplies<double>());
}

Vector Vector::operator/(const Vector& other) const {
    return binary_operation(other, std::divides<double>());
}

Vector Vector::binary_operation(const Vector& other, std::function<double(double, double)> op) const {
    if (data.size() != other.data.size()) {
        throw std::invalid_argument("Vectors must have the same length");
    }
    std::vector<double> result_data(data.size());
    std::transform(data.begin(), data.end(), other.data.begin(), result_data.begin(), op);
    return Vector(data.size(), result_data);
}