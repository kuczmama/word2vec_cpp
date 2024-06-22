#include "word2vec.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>  // For std::isspace
#include <random>
#include <thread>
#include <mutex>
#include <vector>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <filesystem>
#include <iterator>

Word2Vec::Word2Vec()
    : window_size(2), vector_size(100), learning_rate(0.025), epochs(10) {
    // Default values could be set here
}

Word2Vec::Word2Vec(const std::string& path, int window, int vec_size, double lr, int ep)
    : corpus_path(path), window_size(window), vector_size(vec_size), learning_rate(lr), epochs(ep) {
    build_vocab();
    // Extract the base name from the path for the corpus
    base_name = std::filesystem::path(path).stem().string();
    vector_file_name = base_name + "-vectors.json";
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

std::string Word2Vec::most_similar(const std::string& reference_word) {
    if (vectors.find(reference_word) == vectors.end()) {
        return ""; // Return empty if word not found
    }

    double max_cosine_similarity = -1.0;
    std::string most_similar_word;
    const Vector& reference_vector = vectors[reference_word];

    for (const auto& pair : vectors) {
        if (pair.first == reference_word) continue; // Skip the word itself
        double cosine_sim = reference_vector.cosine_similarity(pair.second);
        if (cosine_sim > max_cosine_similarity) {
            max_cosine_similarity = cosine_sim;
            most_similar_word = pair.first;
        }
    }
    return most_similar_word;
}

void Word2Vec::negative_sampling(const std::string& target, const std::string& context, int num_negative) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, vocab.size() - 1);

    Vector& target_vector = vectors[target];
    Vector& context_vector = vectors[context];

    // Positive sample
    double dot_product = target_vector.dot(context_vector);
    double gradient = learning_rate * (1 - sigmoid(dot_product));
    
    target_vector += context_vector * gradient;
    context_vector += target_vector * gradient;

    // Negative samples
    for (int i = 0; i < num_negative; ++i) {
        std::string negative_word;
        do {
            auto it = std::next(vocab.begin(), dis(gen));
            negative_word = *it;
        } while (negative_word == context);

        Vector& negative_vector = vectors[negative_word];
        dot_product = target_vector.dot(negative_vector);
        gradient = learning_rate * (0 - sigmoid(dot_product));

        target_vector += negative_vector * gradient;
        negative_vector += target_vector * gradient;
    }
}

double Word2Vec::sigmoid(double x) {
    return 1.0 / (1.0 + std::exp(-x));
}



void Word2Vec::save_vectors() {
    std::ofstream out(vector_file_name);
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
    std::cout << "Vectors saved to " << vector_file_name << std::endl;
}

void trim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}


bool Word2Vec::load_vectors(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return false;
    }

    std::string line;
    try {
        while (getline(file, line)) {
            std::istringstream iss(line);
            std::string key, token;
            if (getline(iss, key, ':')) {
                trim(key);  // Remove whitespace and quotes around the word
                if (key.front() == '\"') key.erase(0, 1);
                if (key.back() == '\"') key.pop_back();

                std::vector<double> values;
                char ch;
                // Skip to the start of the array
                while (iss >> ch && ch != '[') {}

                // Read the vector values
                while (iss >> ch && ch != ']') {
                    if (std::isdigit(ch) || ch == '-' || ch == '.') {
                        iss.unget();
                        double val;
                        if (iss >> val) {
                            values.push_back(val);
                        }
                        iss >> ch;  // Read past the comma or the closing bracket
                    }
                }

                vectors[key] = Vector(values.size(), values);
            }
        }
        std::cout << "Vectors loaded successfully from " << path << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error loading vectors: " << e.what() << std::endl;
        return false;
    }
}

void Word2Vec::train() {
    int start_epoch = load_epoch_progress();
    for (int epoch = start_epoch; epoch < epochs; ++epoch) {
        int fd = open(corpus_path.c_str(), O_RDONLY);
        if (fd == -1) {
            std::cerr << "Error opening file: " << corpus_path << std::endl;
            return;
        }

        struct stat sb;
        if (fstat(fd, &sb) == -1) {
            std::cerr << "Error getting file size" << std::endl;
            close(fd);
            return;
        }

        char* file_memory = static_cast<char*>(mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
        if (file_memory == MAP_FAILED) {
            std::cerr << "Error mapping file" << std::endl;
            close(fd);
            return;
        }

        const char* begin = file_memory;
        const char* end = begin + sb.st_size;

        std::vector<std::thread> threads;
        int num_threads = std::thread::hardware_concurrency();
        size_t chunk_size = sb.st_size / num_threads;

        for (int i = 0; i < num_threads; ++i) {
            const char* chunk_begin = begin + i * chunk_size;
            const char* chunk_end = (i == num_threads - 1) ? end : chunk_begin + chunk_size;

            threads.emplace_back([this, chunk_begin, chunk_end, epoch]() {
                std::string line;
                const char* pos = chunk_begin;
                int line_number = 0;

                while (pos < chunk_end) {
                    const char* line_end = std::find(pos, chunk_end, '\n');
                    line.assign(pos, line_end);
                    process_line(line, line_number, epoch);
                    pos = line_end + 1;
                    line_number++;
                }
            });
        }

        for (auto& t : threads) {
            t.join();
        }

        munmap(file_memory, sb.st_size);
        close(fd);

        save_epoch_progress(epoch);
        save_vectors();
        std::cout << "Epoch " << epoch + 1 << "/" << epochs << " completed." << std::endl;
    }
    cleanup_progress_files();
}

void Word2Vec::process_line(const std::string& line, int line_number, int epoch) {
    std::istringstream iss(line);
    std::vector<std::string> words{
        std::istream_iterator<std::string>{iss},
        std::istream_iterator<std::string>{}
    };
    for (size_t i = 0; i < words.size(); ++i) {
        if (vectors.find(words[i]) == vectors.end()) continue;
        int start_context = std::max(0, int(i) - window_size);
        int end_context = std::min(int(words.size() - 1), int(i) + window_size);
        for (int j = start_context; j <= end_context; ++j) {
            if (i == static_cast<size_t>(j) || vectors.find(words[j]) == vectors.end()) continue;
            update_vectors(words[i], words[j]);
        }
    }
}

void Word2Vec::update_vectors(const std::string& target, const std::string& context) {
    negative_sampling(target, context, 5); // Use 5 negative samples
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
