#include "word2vec.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

Word2Vec::Word2Vec(const std::string& path, int window, int vec_size, double lr, int ep)
    : corpus_path(path), window_size(window), vector_size(vec_size), learning_rate(lr), epochs(ep) {
    build_vocab();
    create_vectors();
}

void Word2Vec::build_vocab() {
    std::ifstream file(corpus_path);
    std::string line, word;
    while (getline(file, line)) {
        std::istringstream iss(line);
        while (iss >> word) {
            vocab.insert(word);
        }
    }
    file.close();
}

void Word2Vec::create_vectors() {
    for (const auto& word : vocab) {
        vectors[word] = Vector(vector_size);
    }
}

void Word2Vec::save_vectors() {
    std::ofstream out("vectors.json");
    if (vectors.empty()) {
        std::cout << "Warning: Attempting to save an empty vector set." << std::endl;
    } else {
        for (const auto& pair : vectors) {
            out << pair.first << ": [";
            for (size_t i = 0; i < pair.second.size(); ++i) {
                out << pair.second[i] << (i < pair.second.size() - 1 ? ", " : "");
            }
            out << "]" << std::endl;
        }
    }
    out.close();
    std::cout << "Vectors saved." << std::endl;
}

bool Word2Vec::load_vectors() {
    std::ifstream file("vectors.json");
    if (!file) {
        return false;
    }
    // Simulated loading logic
    return true;
}

void Word2Vec::train() {
    int start_epoch = load_epoch_progress();
    for (int epoch = start_epoch; epoch < epochs; ++epoch) {
        std::ifstream file(corpus_path);
        std::string line;
        int line_number = 0;
        while (getline(file, line)) {
            process_line(line, line_number, epoch);
            line_number++;
        }
        file.close();
        save_epoch_progress(epoch);
        save_vectors();
        std::cout << "Epoch " << epoch + 1 << "/" << epochs << " completed." << std::endl;
    }
    cleanup_progress_files();
}

void Word2Vec::process_line(const std::string& line, int line_number, int epoch) {
    std::istringstream iss(line);
    std::vector<std::string> words(std::istream_iterator<std::string>{iss},
                                   std::istream_iterator<std::string>{});
    for (size_t i = 0; i < words.size(); ++i) {
        if (vectors.find(words[i]) == vectors.end()) continue;
        int start_context = std::max(0, int(i) - window_size);
        int end_context = std::min(int(words.size() - 1), int(i) + window_size);
        for (int j = start_context; j <= end_context; ++j) {
            if (i == j || vectors.find(words[j]) == vectors.end()) continue;
            update_vectors(words[i], words[j]);
        }
    }
}

void Word2Vec::update_vectors(const std::string& target, const std::string& context) {
    Vector& target_vector = vectors[target];
    Vector& context_vector = vectors[context];
    for (size_t idx = 0; idx < target_vector.size(); ++idx) {
        double gradient = target_vector[idx] - context_vector[idx];
        target_vector[idx] -= learning_rate * gradient;
        context_vector[idx] -= learning_rate * gradient;
    }
}

void Word2Vec::save_epoch_progress(int epoch) {
    std::ofstream out("epoch_progress.txt");
    out << epoch;
    out.close();
}

int Word2Vec::load_epoch_progress() {
    std::ifstream in("epoch_progress.txt");
    int epoch = 0;
    if (in) {
        in >> epoch;
    }
    return epoch;
}

void Word2Vec::save_sentence_progress(int index) {
    std::ofstream out("sentence_progress.txt");
    out << index;
    out.close();
}

int Word2Vec::load_sentence_progress() {
    std::ifstream in("sentence_progress.txt");
    int index = 0;
    if (in) {
        in >> index;
    }
    return index;
}

void Word2Vec::cleanup_progress_files() {
    std::remove("epoch_progress.txt");
    std::remove("sentence_progress.txt");
}
