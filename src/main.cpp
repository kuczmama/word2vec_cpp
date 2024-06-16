#include "vector.h"
#include <iostream>

int main() {
    // Example usage of the Vector class
    try {
        Vector v1(5);
        Vector v2(5);

        Vector v3 = v1 + v2;
        std::cout << "Dot product: " << v1.dot(v2) << std::endl;
        std::cout << "Cosine similarity: " << v1.cosine_similarity(v2) << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
