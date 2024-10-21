
#ifndef DATA_HANDLE_H
#define DATA_HANDLE_H

DATA_HANDLE_H
#define DATA_HANDLE_H
#include "parquet_handle.hpp"
#include <filesystem>
#include <fstream>
#include <ostream>
#include <string>

namespace fs = std::filesystem;

json load_json(const std::string &filename);

void setup_data(std::string &dataset, std::string &cleaned_data);

#endif // !DATA_HANDLE_H
