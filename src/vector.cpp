#include "vector.h"
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <numeric>

Vector::Vector() : data() {}

Vector::Vector(int size, const std::vector<double>& initial_data) {
    if (initial_data.empty()) {
        data.resize(size);
        std::generate(data.begin(), data.end(), []() {
            double value = (rand() / (double)RAND_MAX) - 0.5;
            // Ensure that value is never exactly zero
            return value == 0.0 ? 0.1 : value;
        });
    } else {
        data = initial_data;
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

size_t Vector::size() const {
    return data.size();
}

double Vector::dot(const Vector& other) const {
    return std::inner_product(data.begin(), data.end(), other.data.begin(), 0.0);
}

double Vector::norm() const {
    return std::sqrt(std::inner_product(data.begin(), data.end(), data.begin(), 0.0));
}

double Vector::cosine_similarity(const Vector& other) const {
    double norm_a = norm();
    double norm_b = other.norm();
    if (norm_a == 0 || norm_b == 0) {
        // If either norm is zero, return a default value or handle it appropriately.
        // Here, we'll return 0.0 to indicate no similarity.
        return 0.0;
    }
    return dot(other) / (norm_a * norm_b);
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
