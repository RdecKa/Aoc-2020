#include "aoclib.hpp"

void printMap(std::unordered_map<unsigned int, unsigned int>& memory) {
  std::cout << "---" << std::endl;
  for (auto& e : memory) {
    std::cout << e.first << ", " << e.second << std::endl;
  }
}

void solve(std::vector<unsigned int>& input, unsigned int limit = 2020) {
  std::unordered_map<unsigned int, unsigned int> memory;
  unsigned int turn = 0;  // Value for 0 will be written twice (step 0 (ignored) and 1)
  unsigned int lastVal = 0;
  for (auto val : input) {
    memory[lastVal] = turn;
    lastVal = val;
    ++turn;
  }
  for (turn; turn < limit; ++turn) {
    auto search = memory.find(lastVal);
    unsigned int val;
    if (search == memory.end()) {
      val = 0;
    } else {
      val = turn - search->second;
    }
    memory[lastVal] = turn;
    lastVal = val;
  }
  std::cout << lastVal << std::endl;
}

int main() {
  // std::vector<unsigned int> small_input = {0, 3, 6};
  // solve(small_input, 10);

  std::vector<unsigned int> input = {0, 6, 1, 7, 2, 19, 20};
  solve(input);
  solve(input, 30000000);  // This one takes a few seconds

  return 0;
}
