#include "include/glove.hpp"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  std::string curDir_file = "/home/herman/workspace/VectorDB/";
  std::string datasetDir_file = curDir_file + "dataset/";
  std::string dataset_file = datasetDir_file + "download/medium_articles/data/";
  std::string cleaned_data_file =
      datasetDir_file + "cleaned/medium_articles1.json";
  std::string output_model = curDir_file + "models/glove_model.json";

  int vector_size = 300;
  int iterations = 50;
  double LR = 0.05;
  double x_max = 100.0;
  double alpha = 0.75;

  // load the cleaned dataset
  json cleaned_data = load_json(cleaned_data_file);

  std::unordered_map<std::string, std::vector<double>> word_vectors;

  // parse through each article in the dataset
  std::cout << "begening to process the data" << std::endl;
  int cnt = 0;
  for (const auto &entry : cleaned_data) {
    std::cout << "@ article no. :" << cnt++ << std::endl;
    std::string article_txt = entry["article"];
    std::vector<std::string> tokens = tokenize_data_article(article_txt);
    std::unordered_map<std::string, std::unordered_map<std::string, double>>
        cooccurrance_matrix = build_cooccurrance_matrix(tokens, 7);
    train_glove_article(cooccurrance_matrix, word_vectors, vector_size,
                        iterations, LR, x_max, alpha);
  }
  std::cout << "completed processing dataset" << std::endl;
  save_word_vec2json(word_vectors, output_model);

  return 0;
}
