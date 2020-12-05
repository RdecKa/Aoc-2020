#include "aoclib.hpp"

int parseInputLine(std::string& line) {
  int id = 0;
  for (const char& c : line) {
    switch (c) {
      case 'F':
      case 'L':
        id *= 2;
        break;
      case 'B':
      case 'R':
        id = id * 2 + 1;
        break;
      default:
        std::cerr << "Invalid character " << c << std::endl;
        break;
    }
  }
  return id;
}

void part1(std::vector<int> input) {
  auto max = *std::max_element(input.begin(), input.end());
  std::cout << max << std::endl;
}

void part2(std::vector<int> input) {
  auto max = *std::max_element(input.begin(), input.end());
  auto occupiedSeats = std::vector<bool>(max);
  for (auto id : input) {
    occupiedSeats[id] = true;
  }
  for (int i = 1; i < max - 1; ++i) {
    if (!occupiedSeats[i] && occupiedSeats[i - 1] && occupiedSeats[i + 1]) {
      std::cout << i << std::endl;
    }
  }
}

int main() {
  const std::string filename = "../day-05/input.txt";
  auto parsed_input = aoc::readParseInput(filename, parseInputLine);

  part1(parsed_input);
  part2(parsed_input);

  return 0;
}
