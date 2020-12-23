#include "aoclib.hpp"

struct Cup {
  const uint32_t value;
  std::shared_ptr<struct Cup> next;

  Cup(const uint32_t value, std::shared_ptr<struct Cup> next) : value(value), next(next) {}
};

class Game {
 protected:
  std::shared_ptr<Cup> currentCup;
  std::vector<std::shared_ptr<Cup>> cups;

  uint32_t selectDestination() {
    size_t numCups = this->cups.size() - 1;
    uint32_t destination = currentCup->value;
    for (bool destinationValid = false; !destinationValid;) {
      destinationValid = true;
      destination = ((destination - 1 + numCups - 1) % numCups) + 1;
      std::shared_ptr<Cup> check(this->currentCup->next);
      for (size_t i = 0; i < 3; ++i) {
        if (check->value == destination) {
          destinationValid = false;
          break;
        }
        check = check->next;
      }
    }
    return destination;
  }

  void playOneRound() {
    std::shared_ptr<Cup> pickUpHead = this->currentCup->next;
    std::shared_ptr<Cup> pickUpTail = pickUpHead->next->next;

    uint32_t destination = this->selectDestination();
    std::shared_ptr<Cup> destinationCup(this->cups[destination]);

    // Rewire
    std::shared_ptr<Cup> afterDestinationCup(destinationCup->next);
    std::shared_ptr<Cup> afterPickUpCup(pickUpTail->next);
    destinationCup->next = pickUpHead;
    pickUpTail->next = afterDestinationCup;
    this->currentCup->next = afterPickUpCup;

    this->currentCup = this->currentCup->next;
  }

 public:
  Game(std::string& cups) {
    this->cups = std::vector<std::shared_ptr<Cup>>(cups.size() + 1, nullptr);

    auto lastCup = std::make_shared<Cup>(static_cast<uint32_t>(cups[cups.size() - 1] - '0'), nullptr);
    this->cups[lastCup->value] = lastCup;

    std::shared_ptr<Cup> nextCup(lastCup);
    for (int i = cups.size() - 2; i >= 0; --i) {
      nextCup = std::make_shared<Cup>(static_cast<uint32_t>(cups[i] - '0'), nextCup);
      this->cups[nextCup->value] = nextCup;
    }

    lastCup->next = nextCup;

    this->currentCup = nextCup;
  }

  ~Game() {
    // Needed to break the cycle of shared pointers and to avoid stack overflow
    // when destructing a BigGame object
    for (size_t i = 1; i < this->cups.size(); ++i) this->cups[i]->next = nullptr;
  }

  void play(size_t nRounds) {
    for (size_t i = 0; i < nRounds; ++i) {
      this->playOneRound();
    }
  }

  std::string getLabels() {
    // Find Cup 1
    std::shared_ptr<Cup> cup(this->cups[1]);

    std::string result = "";
    cup = cup->next;
    while (cup->value != 1) {
      result += std::to_string(cup->value);
      cup = cup->next;
    }
    return result;
  }

  friend std::ostream& operator<<(std::ostream& output, const Game& g) {
    output << "(" << g.currentCup->value << ") ";
    std::shared_ptr<Cup> cup = g.currentCup->next;
    int c = 0;
    while (cup != g.currentCup) {
      if (c < 20 || c > 1000000 - 20) output << cup->value << " ";
      cup = cup->next;
      ++c;
    }
    return output;
  }
};

class BigGame : public Game {
 public:
  BigGame(std::string& cups) : Game(cups) {
    uint32_t numCups = 1000000;
    this->cups.resize(numCups + 1, nullptr);

    std::shared_ptr<Cup> lastCup = this->currentCup;
    while (lastCup->next != this->currentCup) lastCup = lastCup->next;

    std::shared_ptr<Cup> nextCup(this->currentCup);
    for (uint32_t cupNum = numCups; cupNum >= 10; --cupNum) {
      nextCup = std::make_shared<Cup>(cupNum, nextCup);
      this->cups[cupNum] = nextCup;
    }

    lastCup->next = nextCup;
  }

  unsigned long getResult() {
    std::shared_ptr<Cup> first = this->cups[1]->next;
    std::shared_ptr<Cup> second = first->next;

    return static_cast<unsigned long>(first->value) * static_cast<unsigned long>(second->value);
  }
};

void part1(std::string& input) {
  Game game(input);
  game.play(100);
  std::cout << game.getLabels() << std::endl;
}

void part2(std::string& input) {
  BigGame game(input);
  game.play(10000000);
  std::cout << game.getResult() << std::endl;
}

int main() {
  std::string input = "942387615";  // My input
  // std::string input = "389125467";  // Test input

  part1(input);
  part2(input);

  return 0;
}
