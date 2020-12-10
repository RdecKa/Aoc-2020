#include "aoclib.hpp"

void part1(std::vector<int> &input) {
  int count1 = 0;
  int count3 = 0;

  for (int i = 0; i < input.size(); ++i) {
    if (input[i + 1] - input[i] == 1) ++count1;
    if (input[i + 1] - input[i] == 3) ++count3;
  }
  std::cout << count1 << "*" << count3 << "=" << count1 * count3 << std::endl;
}

long numArrangements(std::vector<int> &input, int start, int end) {
  if (start == end) return 1;
  long num = 0;

  for (int i = 1; i <= 3; ++i) {
    if (start + i <= end && input[start + i] - input[start] <= 3) {
      if (i == 3) {
        ++num;
      } else {
        num += numArrangements(input, start + i, end);
      }
    }
  }

  return num;
}

void part2(std::vector<int> &input) {
  int idxStart = 0;
  int idxEnd = 0;
  unsigned long num = 1;
  while (idxStart < input.size() - 1) {
    while (idxEnd < input.size() && input[idxEnd + 1] - input[idxEnd] <= 2) {
      ++idxEnd;
    }
    num *= numArrangements(input, idxStart, idxEnd);
    ++idxEnd;
    idxStart = idxEnd;
  }
  std::cout << num << std::endl;
}

int main() {
  const std::string filename = "../day-10/input.txt";
  auto input = aoc::readIntInput(filename);
  input.push_back(0);
  std::sort(input.begin(), input.end());
  input.push_back(input[input.size() - 1] + 3);

  part1(input);
  part2(input);

  return 0;
}
