#include "aoclib.hpp"

class Group {
 public:
  std::set<std::set<char>> answers;

  Group(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    auto it = begin;
    std::set<std::set<char>> groupSet;
    while (it != end) {
      std::set<char> s;
      for (char &c : *it) {
        s.insert(c);
      }
      groupSet.insert(s);
      ++it;
    }
    this->answers = groupSet;
  }

  int answeredAny() {
    std::set<char> answersUnion;
    for (auto person : this->answers) {
      answersUnion.merge(person);
    }
    return answersUnion.size();
  }

  int answeredAll() {
    std::set<char> answersIntersection = *this->answers.begin();
    for (auto person : this->answers) {
      std::set<char> tmp;
      std::set_intersection(answersIntersection.begin(), answersIntersection.end(), person.begin(), person.end(),
                            std::inserter(tmp, tmp.end()));
      answersIntersection = tmp;
    }
    return answersIntersection.size();
  }
};

void part1(std::vector<Group> input) {
  int sum = 0;
  for (auto g : input) {
    sum += g.answeredAny();
  }
  std::cout << sum << std::endl;
}

void part2(std::vector<Group> input) {
  int sum = 0;
  for (auto g : input) {
    sum += g.answeredAll();
  }
  std::cout << sum << std::endl;
}

std::vector<Group> createGroups(std::vector<std::string> input) {
  auto v = std::vector<Group>();
  auto it = input.begin();
  while (it != input.end()) {
    auto begin = it;
    while ((*it).length() > 0 && it != input.end()) {
      ++it;
    }
    auto p = Group(begin, it);
    v.push_back(p);
    if (it != input.end()) {
      ++it;
    }
  }
  return v;
}

int main() {
  const std::string filename = "../day-06/input.txt";
  auto parsed_input = aoc::readStringInput(filename);
  auto groups = createGroups(parsed_input);

  part1(groups);
  part2(groups);

  return 0;
}
