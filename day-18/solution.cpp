#include "aoclib.hpp"

enum class Operator { PLUS = '+', MULT = '*' };

class Expression {
 public:
  virtual ~Expression() = default;
  virtual unsigned long eval() const = 0;
  virtual std::string str() const = 0;
};

class ExpressionBinary : public Expression {
 private:
  std::unique_ptr<Expression> left;
  std::unique_ptr<Expression> right;
  Operator op;

 public:
  ExpressionBinary(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right, Operator op)
      : left(std::move(left)), right(std::move(right)), op(op) {}

  unsigned long eval() const {
    unsigned long valLeft = this->left->eval();
    unsigned long valRight = this->right->eval();
    switch (this->op) {
      case Operator::PLUS:
        return valLeft + valRight;
      case Operator::MULT:
        return valLeft * valRight;
      default:
        std::cerr << "Invalid operator" << std::endl;
        std::terminate();
    }
  }

  std::string str() const {
    std::string opStr = std::string(1, static_cast<char>(this->op));
    return "(" + this->left->str() + opStr + this->right->str() + ")";
  }
};

class Value : public Expression {
 private:
  unsigned long val;

 public:
  Value(int val) : val(val) {}
  unsigned long eval() const { return this->val; }
  std::string str() const { return std::to_string(this->val); }
};

std::pair<std::unique_ptr<Expression>, int> parseExpression(std::string &line, int start) {
  std::unique_ptr<Expression> root = std::make_unique<Value>(0);
  Operator op = Operator::PLUS;
  int i = start;
  while (i < line.size()) {
    char token = line[i];
    int token_int = static_cast<int>(token);
    if (isdigit(token_int)) {
      std::unique_ptr<Expression> right = std::make_unique<Value>(token_int - '0');
      root = std::make_unique<ExpressionBinary>(std::move(root), std::move(right), op);
    } else if (token == '+') {
      op = Operator::PLUS;
    } else if (token == '*') {
      op = Operator::MULT;
    } else if (token == ' ') {
      ++i;
      continue;
    } else if (token == ')') {
      return std::pair(std::move(root), i);
    } else if (token == '(') {
      std::pair<std::unique_ptr<Expression>, int> result = parseExpression(line, i + 1);
      std::unique_ptr<Expression> right = std::move(result.first);
      i = result.second;
      root = std::make_unique<ExpressionBinary>(std::move(root), std::move(right), op);
    } else {
      std::cerr << "Invalid token " << token << std::endl;
      std::terminate();
    }
    ++i;
  }

  return std::pair(std::move(root), i);
}

// Insert parentheses around expressions with +
std::string insertParantheses(std::string &line) {
  std::string newLine = line;
  int i = 0;
  while (i < newLine.size()) {
    if (newLine[i] != '+') {
      ++i;
      continue;
    }

    // Find end of the expr (to the right of +)
    size_t posEnd = i + 1;
    int parDiff = 0;
    char lastRead = newLine[posEnd];
    while (!(parDiff == 0 && (lastRead == ')' || isdigit(lastRead))) && posEnd < newLine.size()) {
      if (newLine[posEnd] == '(') ++parDiff;
      if (newLine[posEnd] == ')') --parDiff;
      lastRead = newLine[posEnd];
      ++posEnd;
    }
    newLine.insert(posEnd, ")");

    // Find beginning of the expr (to the left of +)
    size_t posStart = i - 1;
    parDiff = 0;
    lastRead = newLine[posStart];
    while (!(parDiff == 0 && (lastRead == '(' || isdigit(lastRead))) && posStart > 0) {
      if (newLine[posStart] == '(') ++parDiff;
      if (newLine[posStart] == ')') --parDiff;
      lastRead = newLine[posStart];
      --posStart;
    }
    if (posStart > 0) ++posStart;
    newLine.insert(posStart, "(");

    i += 2;  // Additional '(' was added before position i, so not just ++i
  }
  return newLine;
}

std::unique_ptr<Expression> parseExpressionWrapper(std::string &line) { return parseExpression(line, 0).first; }

std::unique_ptr<Expression> parseExpressionWrapper2(std::string &line) {
  std::string newLine = insertParantheses(line);
  return parseExpression(newLine, 0).first;
}

void solve(std::vector<std::unique_ptr<Expression>> &input) {
  unsigned long sum = 0;
  for (auto &e : input) {
    sum += static_cast<unsigned long>(e->eval());
  }
  std::cout << sum << std::endl;
}

int main() {
  const std::string filename = "../day-18/input.txt";

  // Part 1
  auto input = aoc::readParseInput(filename, parseExpressionWrapper);
  solve(input);

  // Part 2
  // Insert parantheses around expressions with +, then solve as before.
  auto input2 = aoc::readParseInput(filename, parseExpressionWrapper2);
  solve(input2);
  return 0;
}
