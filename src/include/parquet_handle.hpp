#ifndef PARQUET_HANDLE_H
#define PARQUET_HANDLE_H

#include <arrow/api.h>
#include <arrow/io/api.h>
#include <arrow/table.h>
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <parquet/api/reader.h>
#include <parquet/arrow/reader.h>

using json = nlohmann::json;

std::string preprocess_text(const std::string &input);

void process_parquet2json(const std::string &input_parquet,
                          const std::string &output_json);

#endif // !PARQUET_HANDLE_H
