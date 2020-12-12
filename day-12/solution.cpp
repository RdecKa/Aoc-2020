#include "aoclib.hpp"

enum class Action { N = 'N', S = 'S', E = 'E', W = 'W', L = 'L', R = 'R', F = 'F' };
enum class Orientation { North = 'N', South = 'S', East = 'E', West = 'W' };

class Instruction {
 private:
  Action action;
  unsigned int value;

 public:
  Instruction(const std::string& str) {
    char c = str[0];
    this->action = static_cast<Action>(c);
    this->value = std::atoi(str.substr(1).c_str());
  }

  Action getAction() const { return action; }
  unsigned int getValue() const { return value; }
};

std::ostream& operator<<(std::ostream& output, const Instruction& instr) {
  return output << static_cast<char>(instr.getAction()) << instr.getValue();
}

class Navigation {
 private:
  int east;
  int north;
  unsigned int orientationIndex;
  const std::array<Orientation, 4> directions{Orientation::North, Orientation::West, Orientation::South,
                                              Orientation::East};

  void moveNorth(unsigned int value) { this->north += value; }
  void moveSouth(unsigned int value) { this->north -= value; }
  void moveEast(unsigned int value) { this->east += value; }
  void moveWest(unsigned int value) { this->east -= value; }

  void rotateLeft(unsigned int value) {
    unsigned int n = value / 90;
    orientationIndex += n;
    orientationIndex %= 4;
  }
  void rotateRight(unsigned int value) {
    unsigned int n = value / 90;
    orientationIndex += 4 - n;
    orientationIndex %= 4;
  }

 public:
  Navigation() : east(0), north(0), orientationIndex(3) {}

  void followInstruction(Instruction& instr) {
    switch (instr.getAction()) {
      case Action::N:
        this->moveNorth(instr.getValue());
        break;
      case Action::W:
        this->moveWest(instr.getValue());
        break;
      case Action::S:
        this->moveSouth(instr.getValue());
        break;
      case Action::E:
        this->moveEast(instr.getValue());
        break;
      case Action::L:
        this->rotateLeft(instr.getValue());
        break;
      case Action::R:
        this->rotateRight(instr.getValue());
        break;
      case Action::F:
        switch (this->directions[this->orientationIndex]) {
          case Orientation::North:
            this->moveNorth(instr.getValue());
            break;
          case Orientation::West:
            this->moveWest(instr.getValue());
            break;
          case Orientation::South:
            this->moveSouth(instr.getValue());
            break;
          case Orientation::East:
            this->moveEast(instr.getValue());
            break;
          default:
            std::cerr << "Invalid orientation" << std::endl;
            break;
        }
        break;
      default:
        std::cerr << "Invalid instruction " << instr << std::endl;
        break;
    }
  }
  int getPoseNorth() const { return this->north; }
  int getPoseEast() const { return this->east; }
  Orientation getOrientation() const { return this->directions[this->orientationIndex]; }
};

std::ostream& operator<<(std::ostream& output, const Navigation& nav) {
  return output << nav.getPoseNorth() << ", " << nav.getPoseEast() << ", " << static_cast<char>(nav.getOrientation());
}

Instruction parseInputLine(std::string& line) { return Instruction(line); }

void part1(std::vector<Instruction>& instructions) {
  Navigation navigation;
  for (auto& instr : instructions) {
    navigation.followInstruction(instr);
  }
  int result = abs(navigation.getPoseNorth()) + abs(navigation.getPoseEast());
  std::cout << result << std::endl;
}

/***** PART 2 *****/
class CorrectNavigation {
 private:
  int waypointNorth;
  int waypointEast;
  int positionNorth;
  int positionEast;

  void moveWaypointNorth(int value) { this->waypointNorth += value; }
  void moveWaypointEast(int value) { this->waypointEast += value; }
  void rotateWaypointLeft90() {
    int tmp = this->waypointEast;
    this->waypointEast = -this->waypointNorth;
    this->waypointNorth = tmp;
  }
  void rotateWaypointLeft(unsigned int value) {
    int n = value / 90;
    for (int i = 0; i < n; ++i) this->rotateWaypointLeft90();
  }
  void rotateWaypointRight(unsigned int value) {
    int n = 4 - value / 90;
    for (int i = 0; i < n; ++i) this->rotateWaypointLeft90();
  }
  void moveShip(unsigned int value) {
    this->positionEast += value * this->waypointEast;
    this->positionNorth += value * this->waypointNorth;
  }

 public:
  CorrectNavigation() : waypointNorth(1), waypointEast(10), positionNorth(0), positionEast(0) {}

  void followInstruction(Instruction& instr) {
    switch (instr.getAction()) {
      case Action::N:
        this->moveWaypointNorth(instr.getValue());
        break;
      case Action::S:
        this->moveWaypointNorth(-instr.getValue());
        break;
      case Action::E:
        this->moveWaypointEast(instr.getValue());
        break;
      case Action::W:
        this->moveWaypointEast(-instr.getValue());
        break;
      case Action::L:
        this->rotateWaypointLeft(instr.getValue());
        break;
      case Action::R:
        this->rotateWaypointRight(instr.getValue());
        break;
      case Action::F:
        this->moveShip(instr.getValue());
        break;
      default:
        std::cerr << "Invalid instruction " << instr << std::endl;
        break;
    }
  }

  std::array<int, 2> getWaypoint() const { return std::array<int, 2>{this->waypointNorth, this->waypointEast}; }
  std::array<int, 2> getPosition() const { return std::array<int, 2>{this->positionNorth, this->positionEast}; }
};

std::ostream& operator<<(std::ostream& output, const CorrectNavigation& nav) {
  std::array<int, 2> waypoint = nav.getWaypoint();
  std::array<int, 2> position = nav.getPosition();
  return output << "waypoint (" << waypoint[0] << ", " << waypoint[1] << "); position (" << position[0] << ", "
                << position[1] << ")";
}
void part2(std::vector<Instruction>& instructions) {
  CorrectNavigation navigation = CorrectNavigation();
  for (auto& instr : instructions) {
    navigation.followInstruction(instr);
  }
  std::array<int, 2> position = navigation.getPosition();
  int result = abs(position[0]) + abs(position[1]);
  std::cout << result << std::endl;
}

int main() {
  const std::string filename = "../day-12/input.txt";
  std::vector<Instruction> instructions = aoc::readParseInput(filename, parseInputLine);

  part1(instructions);
  part2(instructions);

  return 0;
}
