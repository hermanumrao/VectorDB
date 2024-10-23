#include "include/data_handle.hpp"
#include "include/glove.hpp"
#include "include/parquet_handle.hpp"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  std::string curDir_file = "/home/herman/workspace/VectorDB/";
  std::string datasetDir_file = curDir_file + "dataset/";
  std::string dataset_file = datasetDir_file + "download/medium_articles/data/";
  std::string cleaned_data_file =
      datasetDir_file + "cleaned/medium_articles.json";
  std::string output_model = curDir_file + "models/glove_model.json";

  int vector_size = 300;
  int iterations = 50;
  double LR = 0.05;
  double x_max = 100.0;
  double alpha = 0.75;

  setup_data(dataset_file, cleaned_data_file);

  train_glove(cleaned_data_file, output_model, vector_size, iterations, LR,
              x_max, alpha);

  return 0;
}
