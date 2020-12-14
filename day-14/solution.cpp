#include "aoclib.hpp"

class Mask {
 private:
  uint64_t powersSetTo0;
  uint64_t powersSetTo1;
  std::vector<uint64_t> powersX;

 public:
  Mask() = default;
  Mask(std::string& line) {
    this->powersSetTo0 = 0;
    this->powersSetTo1 = 0;
    this->powersX = std::vector<uint64_t>();

    std::string mask = line.substr(7);
    uint64_t p = 1;

    for (int i = 0; i < mask.size(); ++i) {
      switch (mask[mask.size() - 1 - i]) {
        case '0':
          this->powersSetTo0 += p;
          break;
        case '1':
          this->powersSetTo1 += p;
          break;
        case 'X':
          this->powersX.push_back(p);
          break;
        default:
          std::cerr << "Invalid mask element: " << mask[mask.size() - 1 - i] << std::endl;
          break;
      }
      p *= 2;
    }
    this->powersSetTo0 = UINT64_MAX - this->powersSetTo0;
  }

  uint64_t getPowersSetTo0() const { return this->powersSetTo0; }
  uint64_t getPowersSetTo1() const { return this->powersSetTo1; }
  std::vector<uint64_t> getPowersX() const { return this->powersX; }
};

std::ostream& operator<<(std::ostream& output, const Mask& m) {
  return output << "0: " << m.getPowersSetTo0() << ", 1: " << m.getPowersSetTo1();
}

class Write {
 private:
  uint64_t memLoc;
  uint64_t val;

 public:
  Write(std::string& line) {
    std::vector<std::string> v;
    boost::split(v, line, boost::is_any_of(" []="));
    this->memLoc = static_cast<uint64_t>(std::stoi(v[1]));
    this->val = static_cast<uint64_t>(std::stoi(v[5]));
  }

  uint64_t getMemLoc() const { return this->memLoc; }
  uint64_t getVal() const { return this->val; }
};

std::ostream& operator<<(std::ostream& output, const Write& w) { return output << w.getMemLoc() << ", " << w.getVal(); }

using Instruction = std::variant<Write, Mask>;

Instruction parseInputLine(std::string& line) {
  switch (line[1]) {
    case 'e':
      return Write(line);
    case 'a':
      return Mask(line);
    default:
      std::cerr << "Invalid instruction";
      std::terminate();
  }
}

class Runtime {
 private:
  std::unordered_map<uint64_t, uint64_t> memory;
  Mask mask;

  uint64_t getMaskedValue(uint64_t value, bool v2) {
    uint64_t result = value;
    if (!v2) result &= this->mask.getPowersSetTo0();
    result |= this->mask.getPowersSetTo1();
    return result;
  }

  void writeToMemory(uint64_t value, uint64_t partiallyFixedAddress, int numFixed = 0) {
    std::vector<uint64_t> powersX = this->mask.getPowersX();
    if (numFixed == powersX.size()) {
      // All bits fixed, write to memory
      this->memory[partiallyFixedAddress] = value;
      return;
    }
    uint64_t currentPower = powersX[numFixed];

    uint64_t address0 = partiallyFixedAddress &= (UINT64_MAX - currentPower);
    writeToMemory(value, address0, numFixed + 1);

    uint64_t address1 = partiallyFixedAddress |= currentPower;
    writeToMemory(value, address1, numFixed + 1);
  }

 public:
  Runtime() {
    this->memory = std::unordered_map<uint64_t, uint64_t>();
    this->mask = Mask();
  }

  void executeInstruction(Instruction& instr, bool v2 = false) {
    if (std::holds_alternative<Write>(instr)) {
      Write& write = std::get<Write>(instr);
      if (v2) {
        uint64_t partiallyFixedAddress = this->getMaskedValue(write.getMemLoc(), v2);
        this->writeToMemory(write.getVal(), partiallyFixedAddress);
      } else {
        this->memory[write.getMemLoc()] = this->getMaskedValue(write.getVal(), v2);
      }
    } else if (std::holds_alternative<Mask>(instr)) {
      this->mask = std::get<Mask>(instr);
    }
  }

  uint64_t getMemorySum() {
    uint64_t sum = 0;
    for (auto& val : this->memory) sum += val.second;
    return sum;
  }

  void printMemory() {
    for (auto& val : this->memory) {
      std::cout << "(" << val.first << ", " << val.second << "), ";
    }
    std::cout << std::endl;
  }
};

void solve(std::vector<Instruction>& instructions, bool v2 = false) {
  Runtime runtime;
  for (auto& instr : instructions) runtime.executeInstruction(instr, v2);
  std::cout << runtime.getMemorySum() << std::endl;
}

int main() {
  const std::string filename = "../day-14/input.txt";
  std::vector<Instruction> input = aoc::readParseInput(filename, parseInputLine);

  solve(input);
  solve(input, true);

  return 0;
}
