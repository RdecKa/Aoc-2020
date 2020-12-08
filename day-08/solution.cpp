#include "solution.hpp"

void nop(Program& p, Instruction& i) { p.changePc(1); }
void jmp(Program& p, Instruction& i) { p.changePc(i.getArg()); }
void acc(Program& p, Instruction& i) {
  p.changeAcc(i.getArg());
  p.changePc(1);
}

Instruction::Instruction(std::string instr, int arg) : arg(arg), type(instr) {
  if (instr == "nop") {
    this->func = &nop;
  } else if (instr == "acc") {
    this->func = &acc;
  } else if (instr == "jmp") {
    this->func = &jmp;
  } else {
    std::cerr << "Invalid instruction " << instr << std::endl;
  }
}

// Return true if loop detected, false otherwise
bool Program::runUntilLoop() {
  while (this->pc < this->instructions.size() && !this->executedInstructions[this->pc]) {
    this->executedInstructions[this->pc] = true;
    this->instructions[this->pc].exec(*this);
  }
  return this->pc < this->instructions.size();
}

bool Instruction::switchType() {
  if (this->type == "acc") {
    return false;
  }
  if (this->type == "nop") {
    this->type = "jmp";
    this->func = &jmp;
  } else if (this->type == "jmp") {
    this->type = "nop";
    this->func = &nop;
  }
  return true;
}

bool Program::changeInstruction(int index) { return this->instructions[index].switchType(); }

Instruction parseInputLine(std::string& line) {
  std::vector<std::string> v;
  boost::split(v, line, boost::is_any_of(" "));
  return Instruction(v[0], std::atoi(v[1].c_str()));
}

void part1(Program program) {
  program.runUntilLoop();
  std::cout << program.getAcc() << std::endl;
}

void part2(Program program) {
  for (int i = 0; i < program.getNumInstructions(); ++i) {
    bool switched = program.changeInstruction(i);
    if (switched) {
      bool looped = program.runUntilLoop();
      if (!looped) {
        std::cout << program.getAcc() << std::endl;
      }
      program.changeInstruction(i);
      program.reset();
    }
  }
  program.runUntilLoop();
}

int main() {
  const std::string filename = "../day-08/input.txt";
  auto parsed_input = aoc::readParseInput(filename, parseInputLine);
  auto program = Program(parsed_input);
  part1(program);
  part2(program);

  return 0;
}
