#include "aoclib.hpp"

namespace aoc {
std::vector<int> readIntInput(const std::string filename) {
  std::fstream file;
  std::vector<int> v;
  file.open(filename, std::ios::in);
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      v.push_back(std::stoi(line));
    }
  } else {
    std::cerr << "Can't open file" << std::endl;
    std::cerr << "PWD: " << std::filesystem::current_path() << std::endl;
  }
  return v;
}
}  // namespace aoc
