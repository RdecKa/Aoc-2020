#include "aoclib.hpp"

std::string parseInputLine(std::string &line) { return line; }

uint64_t countTrees(std::vector<std::string> input, int stepHor, int stepVer) {
  int numTrees = 0;
  int idxH = 0;
  int len = input[0].size();
  int idxV = 0;

  while (idxV < input.size()) {
    if (input[idxV][idxH] == '#') {
      ++numTrees;
    }
    idxH = (idxH + stepHor) % len;
    idxV += stepVer;
  }
  return numTrees;
}

void part1(std::vector<std::string> &input) { std::cout << countTrees(input, 3, 1) << std::endl; }

void part2(std::vector<std::string> &input) {
  uint64_t n1 = countTrees(input, 1, 1);
  uint64_t n3 = countTrees(input, 3, 1);
  uint64_t n5 = countTrees(input, 5, 1);
  uint64_t n7 = countTrees(input, 7, 1);
  uint64_t n2 = countTrees(input, 1, 2);
  uint64_t result = n1 * n3 * n5 * n7 * n2;
  std::cout << result << std::endl;
}

int main() {
  const std::string filename = "../day-03/input.txt";
  auto parsed_input = aoc::readParseInput(filename, parseInputLine);

  part1(parsed_input);
  part2(parsed_input);

  return 0;
}
