#include "word2vec.h"
#include <iostream>

int main() {
    Word2Vec word2vec("vectors.json"); // Assuming vectors.json contains trained vectors
    std::string reference_word;
    std::cout << "Enter a word to find similar words: ";
    std::cin >> reference_word;

    std::string similar_word = word2vec.most_similar(reference_word);
    std::cout << "The word most similar to '" << reference_word << "' is '" << similar_word << "'." << std::endl;

    return 0;
}
