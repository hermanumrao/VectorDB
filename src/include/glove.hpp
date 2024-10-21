#ifndef GLOVE_H
#define GLOVE_H

#include "parquet_handle.hpp"
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::string> tokenize_data_article(const std::string &text);

std::unordered_map<std::string, std::unordered_map<std::string, double>>
build_cooccurrance_matrix(const std::vector<std::string> &tokens,
                          int window_size);

void initialize_vector(std::vector<double> &vec);

void train_glove(
    const std::unordered_map<std::string,
                             std::unordered_map<std::string, double>>
        &cooccurrance_matrix,
    std::unordered_map<std::string, std::vector<double>> &word_vectors,
    int vector_size, int iterations, double LR, double x_max, double alpha);

void save_word_vec2json(
    const std::unordered_map<std::string, std::vector<double>> &word_vectors,
    const std::string &output_file);

#endif // !GLOVE_H
