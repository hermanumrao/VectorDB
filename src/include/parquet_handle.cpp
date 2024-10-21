#include "parquet_handle.hpp"

// Preprocess text function
std::string preprocess_text(const std::string &input) {
  std::ofstream outfile;
  std::string result;
  int space_flag = 0;

  for (char c : input) {
    if (std::isalnum(static_cast<unsigned char>(c)) || c == '.' || c == ',' ||
        c == '?' || c == '-' || c == ' ' || c == '\n') {
      if (c == ' ')
        space_flag += 1;
      else
        space_flag = 0;
      if (c == '.')
        result += " <fullstop>";
      else if (c == ',')
        result += " <comma>";
      else if (c == '?')
        result += " <question>";
      else if (space_flag < 2)
        result += c;
    }
  }
  return result;
}

void process_parquet2json(const std::string &input_parquet,
                          const std::string &output_json) {
  // Open the Parquet file
  std::shared_ptr<arrow::io::ReadableFile> infile;
  PARQUET_ASSIGN_OR_THROW(infile, arrow::io::ReadableFile::Open(input_parquet));

  // Create a Parquet file reader
  std::unique_ptr<parquet::arrow::FileReader> parquet_reader;
  PARQUET_THROW_NOT_OK(parquet::arrow::OpenFile(
      infile, arrow::default_memory_pool(), &parquet_reader));

  // Read the entire table from the Parquet file
  std::shared_ptr<arrow::Table> table;
  PARQUET_THROW_NOT_OK(parquet_reader->ReadTable(&table));

  // Locate the columns of interest: "title" and "article"
  int title_idx = table->schema()->GetFieldIndex("title");
  int article_idx = table->schema()->GetFieldIndex("text");

  if (title_idx == -1 || article_idx == -1) {
    std::cerr << "Required columns (title, article) not found!" << std::endl;
    return;
  }

  // Convert the columns to StringArray for easier access
  auto article_column = std::dynamic_pointer_cast<arrow::StringArray>(
      table->column(article_idx)->chunk(0));
  auto title_column = std::dynamic_pointer_cast<arrow::StringArray>(
      table->column(title_idx)->chunk(0));
  // auto article_column = std::dynamic_pointer_cast<arrow::StringArray>(
  //     table->column(article_idx)->chunk(0));

  // Check if column conversion was successful
  if (!title_column || !article_column) {
    std::cerr << "Failed to convert columns to StringArray." << std::endl;
    return;
  }

  // JSON object to hold the processed data
  json output_data;

  // Loop through each entry in the columns, preprocess, and add to the JSON
  // object
  for (int64_t i = 0; i < title_column->length(); ++i) {
    if (title_column->IsValid(i) && article_column->IsValid(i)) {
      std::string title = preprocess_text(title_column->GetString(i));
      std::string article = preprocess_text(article_column->GetString(i));
      output_data.push_back({{"title", title}, {"article", article}});
    }
  }

  // Read existing JSON data if the file already exists
  json existing_data;
  std::ifstream in_file(output_json);
  if (in_file.is_open()) {
    in_file >> existing_data;
    in_file.close();
  }

  // Append the new data to the existing data
  for (auto &entry : output_data) {
    existing_data.push_back(entry);
  }

  // Write the combined data back to the JSON file
  std::ofstream out_file(output_json);
  out_file << existing_data.dump(4); // Pretty-print with 4 spaces
}
