#ifndef AOCLIB_H_
#define AOCLIB_H_

#include <math.h>

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <variant>
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
    std::cerr << "Can't open file " << filename << std::endl;
    std::cerr << "PWD: " << std::filesystem::current_path() << std::endl;
  }
  return v;
}

// Reads a file that contains one integer per line
std::vector<int> readIntInput(const std::string &filename);
std::vector<int> readUnsignedLongInput(const std::string &filename);

std::vector<std::string> readStringInput(const std::string &filename);

template <class T>
void print2dVector(std::vector<std::vector<T>> &grid) {
  for (auto row : grid) {
    for (auto cell : row) {
      std::cout << cell;
    }
    std::cout << std::endl;
  }
}

}  // namespace aoc

#endif
