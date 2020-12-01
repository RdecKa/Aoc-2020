#ifndef AOCLIB_H_
#define AOCLIB_H_

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace aoc {
// Reads a file that contains one integer per line
std::vector<int> read_int_input(const std::string filename);
}  // namespace aoc

#endif
