#include "word2vec.h"
#include <iostream>
#include <cstdlib> // for std::atoi and std::atof

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <corpus_file> [window_size=2] [vector_size=100] [learning_rate=0.025] [epochs=10]" << std::endl;
        return 1;
    }
    
    std::string corpus_path = argv[1];
    int window_size = (argc > 2) ? std::atoi(argv[2]) : 2;
    int vector_size = (argc > 3) ? std::atoi(argv[3]) : 100;
    double learning_rate = (argc > 4) ? std::atof(argv[4]) : 0.025;
    int epochs = (argc > 5) ? std::atoi(argv[5]) : 10;

    Word2Vec word2vec(corpus_path, window_size, vector_size, learning_rate, epochs);
    word2vec.train();
    std::cout << "Training completed successfully." << std::endl;

    return 0;
}
