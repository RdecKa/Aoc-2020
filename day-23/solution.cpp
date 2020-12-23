#include "aoclib.hpp"

struct Cup {
  const short value;
  std::shared_ptr<struct Cup> next;

  Cup(const short value, std::shared_ptr<struct Cup> next) : value(value), next(next) {}
};

class Game {
 private:
  std::shared_ptr<Cup> currentCup;

  short selectDestination() {
    short destination = currentCup->value;
    for (bool destinationValid = false; !destinationValid;) {
      destinationValid = true;
      destination = ((destination - 1 + 8) % 9) + 1;
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

    short destination = this->selectDestination();

    // Find destination
    std::shared_ptr<Cup> destinationCup(pickUpTail);
    while (destinationCup->value != destination) {
      destinationCup = destinationCup->next;
    }

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
    auto lastCup = std::make_shared<Cup>(static_cast<short>(cups[cups.size() - 1] - '0'), nullptr);
    std::shared_ptr<Cup> nextCup(lastCup);
    for (int i = cups.size() - 2; i >= 0; --i) {
      nextCup = std::make_shared<Cup>(static_cast<short>(cups[i] - '0'), nextCup);
    }

    lastCup->next = nextCup;

    this->currentCup = nextCup;
  }

  ~Game() { this->currentCup->next = nullptr; }

  void play(size_t nRounds) {
    for (size_t i = 0; i < nRounds; ++i) {
      this->playOneRound();
      // std::cout << *this << std::endl;
    }
  }

  std::string getLabels() {
    // Find Cup 1
    std::shared_ptr<Cup> cup(this->currentCup);
    while (cup->value != 1) cup = cup->next;

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
    while (cup != g.currentCup) {
      output << cup->value << " ";
      cup = cup->next;
    }
    return output;
  }
};

void part1(std::string& input) {
  Game game(input);
  game.play(100);
  std::cout << game.getLabels() << std::endl;
}

void part2(std::string& input) {}

int main() {
  std::string input = "942387615";  // My input
  // std::string input = "389125467";  // Test input

  part1(input);
  part2(input);

  return 0;
}
