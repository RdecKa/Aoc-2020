#include "aoclib.hpp"

namespace aoc {
std::string parseStringInputLine(std::string &line) { return line; }
int parseIntInputLine(std::string &line) { return std::stoi(line); }

std::vector<int> readIntInput(const std::string &filename) { return readParseInput(filename, parseIntInputLine); }

std::vector<std::string> readStringInput(const std::string &filename) {
  return readParseInput(filename, parseStringInputLine);
}

}  // namespace aoc
