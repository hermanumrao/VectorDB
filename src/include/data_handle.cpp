#include "data_handle.hpp"
#include <iostream>
#include <unistd.h>

json load_json(const std::string &filename) {
  std::ifstream file(filename);
  json data;
  if (file.is_open()) {
    file >> data;
    file.close();
  } else
    std::cerr << "missing json file: " << filename;
  return data;
}

void setup_data(std::string &dataset, std::string &cleaned_data) {
  std::cout << "starting to read datasetfile at : " << dataset << std::endl;
  std::fstream outfile(cleaned_data);
  if (access(cleaned_data.c_str(), F_OK) != 0) {
    for (const auto &entry : fs::directory_iterator(dataset)) {
      if (entry.is_regular_file() && entry.path().extension() == ".parquet") {
        std::string parquet_file = entry.path().string();
        std::cout << "Processing file: " << parquet_file << std::endl;
        process_parquet2json(parquet_file, cleaned_data);
        std::cout << "Processing complete. Cleaned data from " << dataset
                  << " saved to " << cleaned_data << std::endl
                  << std::endl;
      }
    }
  } else
    std::cout << "found json file for dataset at :" << cleaned_data
              << "\n stopped preprocessing, moving on to training data"
              << std::endl;
}
