#include "aoclib.hpp"

class Instruction;

class Program {
 private:
  int pc;
  int acc;
  std::vector<Instruction> instructions;
  std::vector<bool> executedInstructions;

 public:
  Program(std::vector<Instruction> instructions) : pc(0), acc(0), instructions(instructions) {
    this->executedInstructions = std::vector<bool>(instructions.size(), false);
  }
  int getAcc() { return this->acc; }
  int getNumInstructions() { return this->instructions.size(); }
  void changeAcc(int arg) { this->acc += arg; }
  void changePc(int arg) { this->pc += arg; }
  bool runUntilLoop();
  void reset() {
    this->pc = 0;
    this->acc = 0;
    this->executedInstructions = std::vector<bool>(instructions.size(), false);
  }
  bool changeInstruction(int index);
};

class Instruction {
 private:
  int arg;
  std::string type;
  std::function<void(Program&, Instruction&)> func;

 public:
  Instruction(std::string instr, int arg);
  int getArg() { return this->arg; }
  std::string getType() { return this->type; }
  void exec(Program& p) { this->func(p, *this); }
  bool switchType();
};
