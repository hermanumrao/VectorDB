#include "glove.hpp"
#include <iostream>
#include <mutex>

// Global mutex definition
std::mutex word_vector_mutex;

// Tokenize article text into individual words
std::vector<std::string> tokenize_data_article(const std::string &text) {

  std::vector<std::string> tokens;
  std::istringstream iss(text);
  std::string word;
  while (iss >> word)
    tokens.push_back(word);

  return tokens;
}

std::unordered_map<std::string, std::unordered_map<std::string, double>>
build_cooccurrance_matrix(const std::vector<std::string> &tokens,
                          int window_size) {

  std::unordered_map<std::string, std::unordered_map<std::string, double>>
      cooccurrance_matrix;

  int tokens_cnt = tokens.size();
  for (int i = 0; i < tokens_cnt; i++) {
    const std::string word = tokens[i];

    // loking at the context, the sliding window with n words
    for (int j = std::max(0, i - window_size);
         j < std::min(tokens_cnt, i + window_size + 1); j++) {
      if (i != j) {
        const std::string cont_word = tokens[j];
        cooccurrance_matrix[word][cont_word] += 1.0 / abs(i - j);
        // division to add weight by distance hence more accuracy
      }
    }
  }
  return cooccurrance_matrix;
}

// initialize the hidden layer vector with random values
void initialize_vector(std::vector<double> &vec) {
  std::mt19937 random_gen(static_cast<unsigned int>(time(0)));
  std::uniform_real_distribution<double> distribution(-0.5, 0.5);

  for (double &val : vec) {
    val = distribution(random_gen);
  }
}
// use the initialized hidden layer vector to train the model
// void train_glove_article(
//     const std::unordered_map<std::string,
//                              std::unordered_map<std::string, double>>
//         &cooccurrance_matrix,
//     std::unordered_map<std::string, std::vector<double>> &word_vectors,
//     int vector_size, int iterations, double LR, double x_max, double alpha) {
//
//   // initializing the word_vectors with random values
//   // if it has not already been initialized
//   for (const auto &pair : cooccurrance_matrix) {
//     if (word_vectors.find(pair.first) == word_vectors.end())
//     // check to see if the word already exists, if ans=1 mean it doesn't
//     exist
//     {
//       word_vectors[pair.first] = std::vector<double>(vector_size);
//       initialize_vector(word_vectors[pair.first]);
//     }
//     for (const auto &context_pair : pair.second) // check context words too
//     {
//       if (word_vectors.find(context_pair.first) == word_vectors.end()) {
//         word_vectors[context_pair.first] = std::vector<double>(vector_size);
//         initialize_vector(word_vectors[context_pair.first]);
//       }
//     }
//   }
//
//   // training the model using gradient descent
//   for (int iter = 0; iter < iterations; iter++) {
//     for (const auto &pair : cooccurrance_matrix) {
//       const std::string &word = pair.first;
//       for (const auto &context_pair : pair.second) {
//         const std::string &context_word = context_pair.first;
//         double conccurrance = context_pair.second;
//
//         std::vector<double> &word_vec = word_vectors[word];
//         std::vector<double> &context_vec = word_vectors[context_word];
//
//         // compute dot product between word vector and context vector
//         double dot_product = 0.0;
//         for (int k = 0; k < vector_size; k++)
//           dot_product += word_vec[k] * context_vec[k];
//
//         double weight =
//             (conccurrance < x_max) ? pow(conccurrance / x_max, alpha) : 1.0;
//         double diff = dot_product - log(conccurrance);
//
//         {
//           std::lock_guard<std::mutex> lock(word_vector_mutex);
//           // Gradient descent update for each dimension
//           for (int k = 0; k < vector_size; k++) {
//             double grad_word = weight * diff * context_vec[k];
//             double grad_context = weight * diff * word_vec[k];
//
//             word_vec[k] -= LR * grad_word;
//             context_vec[k] -= LR * grad_context;
//           }
//         }
//       }
//     }
//   }
// }
//

void train_glove_article(
    const std::unordered_map<std::string,
                             std::unordered_map<std::string, double>>
        &cooccurrance_matrix,
    std::unordered_map<std::string, std::vector<double>> &word_vectors,
    int vector_size, int iterations, double LR, double x_max, double alpha) {

  // Initialize word vectors
  for (const auto &pair : cooccurrance_matrix) {
    const std::string &word = pair.first;

    if (word_vectors.find(word) == word_vectors.end()) {
      word_vectors[word] = std::vector<double>(vector_size);
      initialize_vector(word_vectors[word]);
    }

    for (const auto &context_pair : pair.second) {
      const std::string &context_word = context_pair.first;

      if (word_vectors.find(context_word) == word_vectors.end()) {
        word_vectors[context_word] = std::vector<double>(vector_size);
        initialize_vector(word_vectors[context_word]);
      }
    }
  }

  // Training loop
  for (int iter = 0; iter < iterations; iter++) {
    for (const auto &pair : cooccurrance_matrix) {
      const std::string &word = pair.first;

      for (const auto &context_pair : pair.second) {
        const std::string &context_word = context_pair.first;
        double conccurrance = context_pair.second;

        // Check if the vectors exist
        if (word_vectors.find(word) == word_vectors.end() ||
            word_vectors.find(context_word) == word_vectors.end()) {
          std::cerr << "Error: One of the words does not exist in the "
                       "word_vectors.\n";
          continue; // Skip this iteration
        }

        std::vector<double> &word_vec = word_vectors[word];
        std::vector<double> &context_vec = word_vectors[context_word];

        // Debugging information
        // std::cout << "Word: " << word << ", Context Word: " << context_word
        // << "\n";

        // Check vector sizes before proceeding
        if (word_vec.size() != vector_size ||
            context_vec.size() != vector_size) {
          std::cerr << "Vector size mismatch! Expected: " << vector_size
                    << ", Word vector size: " << word_vec.size()
                    << ", Context vector size: " << context_vec.size() << "\n";
          throw std::runtime_error("Vector size mismatch!");
        }

        // Compute dot product
        double dot_product = std::inner_product(
            word_vec.begin(), word_vec.end(), context_vec.begin(), 0.0);

        double weight =
            (conccurrance < x_max) ? pow(conccurrance / x_max, alpha) : 1.0;
        double diff = dot_product - log(conccurrance);

        // Locking for thread safety
        {
          std::lock_guard<std::mutex> lock(word_vector_mutex);
          // Gradient descent update for each dimension
          for (int k = 0; k < vector_size; k++) {
            double grad_word = weight * diff * context_vec[k];
            double grad_context = weight * diff * word_vec[k];

            word_vec[k] -= LR * grad_word;
            context_vec[k] -= LR * grad_context;
          }
        }
      }
    }
  }
}

void train_glove(const std::string &cleaned_data_file,
                 const std::string &output_model, int vector_size,
                 int iterations, double LR, double x_max, double alpha) {
  // load the cleaned dataset
  json cleaned_data = load_json(cleaned_data_file);

  // multithreaded to improve speed and efficiency

  std::vector<std::thread> threads;

  std::unordered_map<std::string, std::vector<double>> word_vectors;

  // Parse through each article in the dataset
  std::cout << "Beginning to process the data" << std::endl;
  std::atomic<int> cnt(0); // Atomic counter to avoid race conditions

  for (const auto &entry : cleaned_data) {
    threads.emplace_back([&] {
      int local_cnt = cnt++;
      std::cout << "@ article no. :" << local_cnt << std::endl;
      std::string article_txt = entry["article"];
      std::vector<std::string> tokens = tokenize_data_article(article_txt);

      std::unordered_map<std::string, std::unordered_map<std::string, double>>
          cooccurrance_matrix = build_cooccurrance_matrix(tokens, 7);
      train_glove_article(cooccurrance_matrix, word_vectors, vector_size,
                          iterations, LR, x_max, alpha);
    });
    // limit the number of threads based on system
    if (threads.size() >= 3) {
      for (auto &t : threads)
        if (t.joinable())
          t.join();
      threads.clear();
    }
  }
  for (auto &t : threads)
    if (t.joinable())
      t.join();

  std::cout << "completed processing dataset" << std::endl;
  save_word_vec2json(word_vectors, output_model);
}

// function to save data for later retriveval
void save_word_vec2json(
    const std::unordered_map<std::string, std::vector<double>> &word_vectors,
    const std::string &output_file) {
  json json_out;

  for (const auto &pair : word_vectors)
    json_out[pair.first] = pair.second; // storing data in json with key as
                                        // word and vectory as the value
  std::ofstream file(output_file);
  if (file.is_open()) {
    file << json_out.dump(4); // pretty print with 4 space indent
    file.close();
    std::cout << "Model file has been stored to " << output_file << std::endl;
  } else
    std::cerr << "unble to open file for writing " << output_file << std::endl;
}
