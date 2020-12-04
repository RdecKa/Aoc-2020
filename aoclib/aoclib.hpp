#ifndef AOCLIB_H_
#define AOCLIB_H_

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace aoc {

// Reads a file and parses each line (using provided function)
template <class T>
std::vector<T> readParseInput(const std::string filename, T func(std::string &)) {
  std::fstream file;
  std::vector<T> v;
  file.open(filename, std::ios::in);
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      v.push_back(func(line));
    }
  } else {
    std::cerr << "Can't open file" << std::endl;
    std::cerr << "PWD: " << std::filesystem::current_path() << std::endl;
  }
  return v;
}

// Reads a file that contains one integer per line
std::vector<int> readIntInput(const std::string &filename);

std::vector<std::string> readStringInput(const std::string &filename);

}  // namespace aoc

#endif
