#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>

#include "aoclib.hpp"

class Policy {
 private:
  int min;
  int max;
  char letter;
  std::string pwd;

 public:
  Policy(int min, int max, char let, std::string pwd) : min(min), max(max), letter(let), pwd(pwd){};
  int getMin() const { return min; }
  int getMax() const { return max; }
  char getLetter() const { return letter; }
  const std::string& getPwd() const { return pwd; }
};

std::ostream& operator<<(std::ostream& output, const Policy& p) {
  return output << "(" << p.getMin() << ", " << p.getMax() << ", " << p.getLetter() << ", " << p.getPwd() << ")";
}

Policy parseInputLine(std::string& line) {
  std::vector<std::string> v;
  boost::split(v, line, boost::is_any_of(" -:"));
  return Policy(std::stoi(v[0]), std::stoi(v[1]), v[2].at(0), v[4]);
}

void part1(std::vector<Policy> input) {
  int count = 0;
  for (auto policy : input) {
    auto pwd = policy.getPwd();
    auto n = std::count(pwd.begin(), pwd.end(), policy.getLetter());
    if (n >= policy.getMin() && n <= policy.getMax()) {
      ++count;
    }
  }
  std::cout << "Correct: " << count << std::endl;
}

void part2(std::vector<Policy> input) {
  int count = 0;
  for (auto policy : input) {
    auto pwd = policy.getPwd();
    auto first = pwd.at(policy.getMin() - 1) == policy.getLetter();
    auto second = pwd.at(policy.getMax() - 1) == policy.getLetter();
    if (first ^ second) {
      ++count;
    }
  }
  std::cout << "Correct: " << count << std::endl;
}

int main() {
  const std::string filename = "../day-02/input.txt";
  auto parsed_input = aoc::readParseInput(filename, parseInputLine);
  part1(parsed_input);
  part2(parsed_input);

  return 0;
}
