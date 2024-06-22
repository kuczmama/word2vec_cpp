#ifndef WORD2VEC_H
#define WORD2VEC_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "vector.h"

class Word2Vec {
private:
    std::string corpus_path;
    std::string base_name; // To store the base name of the corpus
    std::string vector_file_name; // To store the resulting vector file name
    int window_size;
    int vector_size;
    double learning_rate;
    int epochs;
    std::unordered_set<std::string> vocab;
    std::unordered_map<std::string, Vector> vectors;
    double sigmoid(double x);
    void negative_sampling(
            const std::string& target,
            const std::string& context,
            int num_negative);

public:
    Word2Vec(); // Default constructor
    Word2Vec(const std::string& path, int window = 2, int vec_size = 100, double lr = 0.025, int ep = 10);
    void build_vocab();
    void create_vectors();
    void save_vectors();
    bool load_vectors(const std::string& path);
    std::string most_similar(const std::string& reference_word);
    void train();
    void process_line(const std::string& line, int line_number, int epoch);
    void update_vectors(const std::string& target, const std::string& context);
    void save_epoch_progress(int epoch);
    int load_epoch_progress();
    void save_sentence_progress(int index);
    int load_sentence_progress();
    void cleanup_progress_files();
};

#endif // WORD2VEC_H
