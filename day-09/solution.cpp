#include "aoclib.hpp"

struct Entry {
  unsigned long val;
  std::vector<unsigned long> sums;
};

std::vector<Entry> calculateSums(std::vector<int> input, unsigned long windowSize) {
  auto vectorWithSums = std::vector<Entry>(input.size());
  for (int i = 0; i < input.size(); ++i) {
    Entry entry;
    entry.val = input[i];
    auto v = std::vector<unsigned long>(std::min(windowSize - 1, input.size() - i - 1));
    for (int j = i + 1; j < i + windowSize && j < input.size(); ++j) {
      v[j - i - 1] = input[i] + input[j];
    }
    entry.sums = v;
    vectorWithSums[i] = entry;
  }
  return vectorWithSums;
}

bool checkIfNumValid(std::vector<Entry> vectorWithSums, int windowSize, int i) {
  for (int j = 1; j <= windowSize; ++j) {
    for (auto s : vectorWithSums[i - j].sums) {
      if (vectorWithSums[i].val == s) {
        return true;
      }
    }
  }
  return false;
}

unsigned long part1(std::vector<Entry> vectorWithSums, int windowSize) {
  for (int i = windowSize; i < vectorWithSums.size(); ++i) {
    if (!checkIfNumValid(vectorWithSums, windowSize, i)) {
      std::cout << vectorWithSums[i].val << std::endl;
      return vectorWithSums[i].val;
    }
  }
  std::cerr << "No solution" << std::endl;
  return 0;
}

void part2(std::vector<Entry> vectorWithSums, int windowSize, unsigned long invalidNumber) {
  unsigned long sum = vectorWithSums[0].val;
  int startIdx = 0;
  int endIdx = 0;
  while (sum != invalidNumber) {
    if (sum < invalidNumber) {
      ++endIdx;
      sum += vectorWithSums[endIdx].val;
    } else if (sum > invalidNumber) {
      sum -= vectorWithSums[startIdx].val;
      ++startIdx;
    }
  }
  unsigned long min = vectorWithSums[startIdx].val;
  unsigned long max = vectorWithSums[startIdx].val;

  for (int i = startIdx; i <= endIdx; ++i) {
    if (vectorWithSums[i].val > max) {
      max = vectorWithSums[i].val;
    } else if (vectorWithSums[i].val < min) {
      min = vectorWithSums[i].val;
    }
  }
  std::cout << max << "+" << min << "=" << (max + min) << std::endl;
}

int main() {
  bool small_input = false;
  std::string filename;
  int windowSize;
  if (small_input) {
    filename = "../day-09/small_input.txt";
    windowSize = 5;
  } else {
    filename = "../day-09/input.txt";
    windowSize = 25;
  }

  auto parsed_input = aoc::readUnsignedLongInput(filename);
  auto vectorWithSums = calculateSums(parsed_input, windowSize);
  unsigned long invalidNumber = part1(vectorWithSums, windowSize);
  part2(vectorWithSums, windowSize, invalidNumber);

  return 0;
}
