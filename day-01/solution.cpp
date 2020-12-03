#include "aoclib.hpp"

void part1(std::vector<int> input) {
  int min = *min_element(std::begin(input), std::end(input));
  int max = *max_element(std::begin(input), std::end(input));

  std::vector<int> mask(max - min + 1);
  for (const auto val : input) {
    int idx = val - min;
    ++mask[idx];
    int idx_inverse = 2020 - val - min;
    if (idx_inverse >= 0 && idx_inverse < mask.size()) {
      ++mask[idx_inverse];
    }
  }

  std::vector<int> result;
  for (int i = 0; i < mask.size(); ++i) {
    if (mask[i] == 2) {
      int r = i + min;
      result.push_back(r);
    }
  }
  std::cout << result[0] << " + " << result[1] << " = " << result[0] + result[1] << std::endl;
  std::cout << result[0] << " * " << result[1] << " = " << result[0] * result[1] << std::endl;
}

void part2(std::vector<int> input) {
  for (auto it1 = input.begin(); it1 != input.end(); ++it1) {
    for (auto it2 = it1 + 1; it2 != input.end(); ++it2) {
      for (auto it3 = it2 + 1; it3 != input.end(); ++it3) {
        if (*it1 + *it2 + *it3 == 2020) {
          std::cout << *it1 << " + " << *it2 << " + " << *it3 << " = " << *it1 + *it2 + *it3 << std::endl;
          std::cout << *it1 << " * " << *it2 << " * " << *it3 << " = " << *it1 * *it2 * *it3 << std::endl;
        }
      }
    }
  }
}

int main() {
  const std::string filename = "../day-01/input.txt";
  auto input = aoc::readIntInput(filename);

  part1(input);
  part2(input);

  return 0;
}
