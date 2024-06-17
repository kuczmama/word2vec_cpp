#include "word2vec.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_vectors.json>" << std::endl;
        return 1;
    }

    Word2Vec model;
    if (!model.load_vectors(argv[1])) {
        std::cerr << "Failed to load vectors from " << argv[1] << std::endl;
        return 1;
    }

    std::cout << "Successfully loaded vectors from " << argv[1] << std::endl;
    std::string input_word;
    while (true) {
        std::cout << "Enter a word to find similar words (or type 'exit' to quit): ";
        std::cin >> input_word;
        if (input_word == "exit") {
            break;
        }

        try {
            std::string similar_word = model.most_similar(input_word);
            if (similar_word.empty()) {
                std::cout << "No similar word found or word not in vocabulary." << std::endl;
            } else {
                std::cout << "The word most similar to '" << input_word << "' is '" << similar_word << "'." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}
