#include "aoclib.hpp"

struct ReturnObject {
  bool success;
  std::vector<size_t> newStarts;  // Next positions where to start parsing
};

class Rule {
 protected:
  size_t ruleIdx;
  Rule(size_t idx) : ruleIdx(idx) {}

 public:
  virtual ReturnObject parse(std::string& message, size_t position) const = 0;
  virtual std::string str() const = 0;
  size_t getIdx() const { return this->ruleIdx; }
  virtual ~Rule() = default;
};

class Letter : public Rule {
 private:
  char letter;

 public:
  Letter(size_t idx, char letter) : Rule(idx), letter(letter) {}
  ReturnObject parse(std::string& message, size_t position) const override {
    if (message[position] == this->letter) {
      return {true, {position + 1}};
    }
    return {false, {}};
  }
  std::string str() const override { return std::to_string(this->ruleIdx) + ": \"" + this->letter + "\""; }
};

class CompositeRule : public Rule {
 private:
  std::vector<std::shared_ptr<Rule>> subrules;

 public:
  CompositeRule(size_t idx, std::vector<std::shared_ptr<Rule>> subrules) : Rule(idx), subrules(subrules) {}

  ReturnObject parse(std::string& message, size_t position) const override {
    if (position >= message.size()) return {false, {}};

    std::vector<size_t> possibleStarts = {position};
    for (auto& rule : this->subrules) {
      std::vector<size_t> possibleNextStarts;
      for (auto& pos : possibleStarts) {
        auto [success, positions] = rule->parse(message, pos);
        if (success) {
          possibleNextStarts.insert(possibleNextStarts.end(), positions.begin(), positions.end());
        }
      }
      possibleStarts = possibleNextStarts;
    }

    return {possibleStarts.size() > 0, possibleStarts};
  }

  std::string str() const {
    std::string subrules = "";
    for (auto& rule : this->subrules) {
      subrules += std::to_string(rule->getIdx()) + " ";
    }
    return std::to_string(this->ruleIdx) + ": " + subrules;
  }
};

class OrRule : public Rule {
 private:
  std::vector<std::shared_ptr<Rule>> options;

 public:
  OrRule(size_t idx) : Rule(idx) {}

  void init(std::vector<std::shared_ptr<Rule>> options) { this->options = options; }

  ReturnObject parse(std::string& message, size_t position) const override {
    if (position >= message.size()) return {false, {}};

    std::vector<size_t> possiblePositions;
    for (auto& option : this->options) {
      auto [success, newStarts] = option->parse(message, position);
      if (success) possiblePositions.insert(possiblePositions.end(), newStarts.begin(), newStarts.end());
    }
    if (possiblePositions.size() > 0) {
      return {true, possiblePositions};
    }
    return {false, {}};
  }

  std::string str() const override {
    std::string out = "";
    for (auto& rule : this->options) {
      out += rule->str() + "| ";
    }
    return out;
  }
};

std::vector<std::shared_ptr<Rule>> createRules(std::vector<std::string>& input) {
  std::vector<std::shared_ptr<Rule>> rulesVector;
  std::vector<std::string> splitValues;
  for (auto& line : input) {
    std::shared_ptr<Rule> newRule;
    boost::split(splitValues, line, boost::is_any_of(":"));
    size_t idx = atoi(splitValues[0].c_str());
    if (splitValues[1][1] == '"') {
      // FORMAT: 32: "a"
      newRule = std::make_shared<Letter>(idx, splitValues[1][2]);
    } else {
      // any other format
      newRule = std::make_shared<OrRule>(idx);
    }
    if (idx >= rulesVector.size()) {
      rulesVector.resize(idx + 1);
    }
    rulesVector[idx] = newRule;
    splitValues.clear();
  }

  // Add pointers
  for (auto& line : input) {
    boost::split(splitValues, line, boost::is_any_of(":"));
    size_t idx = atoi(splitValues[0].c_str());

    if (splitValues[1][1] != '"') {  // Simple rules (letters) handeled above
      std::string rules = splitValues[1];
      splitValues.clear();
      boost::split(splitValues, rules, boost::is_any_of("|"));
      std::vector<std::shared_ptr<Rule>> orComposites;

      for (auto& ruleStr : splitValues) {
        std::vector<std::string> v;
        boost::split(v, ruleStr, boost::is_any_of(" "));
        std::vector<std::shared_ptr<Rule>> composites;

        for (auto& idxStr : v) {
          if (idxStr != "") {
            composites.push_back(rulesVector[std::atoi(idxStr.c_str())]);
          }
        }

        orComposites.push_back(std::make_shared<CompositeRule>(CompositeRule(idx, composites)));
      }

      auto& r = static_cast<OrRule&>(*rulesVector[idx]);
      r.init(orComposites);
    }
    splitValues.clear();
  }
  return rulesVector;
}

std::vector<std::string> transformRules(std::vector<std::string> input) {
  for (int i = 0; i < input.size(); ++i) {
    if (input[i] == "8: 42") {
      input[i] = "8: 42 | 42 8";
    } else if (input[i] == "11: 42 31") {
      input[i] = "11: 42 31 | 42 11 31";
    }
  }
  return input;
}

std::pair<std::vector<std::string>, std::vector<std::string>> splitInput(std::vector<std::string>& input) {
  auto it = input.begin();
  while (*it != "") ++it;
  auto rules = std::vector<std::string>(input.begin(), it);

  ++it;
  auto messagesBegin = it;
  while (it != input.end()) ++it;
  auto messages = std::vector<std::string>(messagesBegin, it);

  return std::pair(rules, messages);
}

void solve(std::vector<std::shared_ptr<Rule>>& rules, std::vector<std::string>& messages) {
  std::shared_ptr<Rule> rule0 = rules[0];
  int valid = 0;
  for (auto& msg : messages) {
    auto [success, positions] = rule0->parse(msg, 0);
    if (!success) continue;
    for (auto p : positions) {
      if (p == msg.size()) {
        ++valid;
        break;
      }
    }
  }
  std::cout << valid << std::endl;
}

int main() {
  const std::string filename = "../day-19/input.txt";
  auto input = aoc::readStringInput(filename);
  auto [rulesStr, messages] = splitInput(input);

  // Star 1
  std::vector<std::shared_ptr<Rule>> rules = createRules(rulesStr);
  solve(rules, messages);

  // Star 2
  std::vector<std::string> transformedRulesStr = transformRules(rulesStr);
  std::vector<std::shared_ptr<Rule>> transformedRules = createRules(transformedRulesStr);
  solve(transformedRules, messages);

  return 0;
}
