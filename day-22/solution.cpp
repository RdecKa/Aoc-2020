#include "aoclib.hpp"

class Hand {
 private:
  std::deque<short> cards;

 public:
  Hand() : cards(std::deque<short>()){};

  std::deque<short> getCards() const { return this->cards; }
  bool empty() const { return this->cards.empty(); }

  void addCard(short card) { this->cards.push_back(card); }

  short getFirstCard() {
    short card = this->cards.front();
    this->cards.pop_front();
    return card;
  }

  unsigned long handValue() const {
    unsigned long value = 0;
    for (unsigned int i = 1; i <= this->cards.size(); ++i) {
      short card = this->cards[this->cards.size() - i];
      value += i * card;
    }
    return value;
  }
};

class Player {
 private:
  unsigned int id;
  Hand hand;

 public:
  Player() = default;
  Player(unsigned int id) : id(id), hand(Hand()) {}

  unsigned int getId() const { return this->id; }
  Hand getHand() const { return this->hand; }
  bool hasLost() const { return this->hand.empty(); }

  void addCard(short card) { this->hand.addCard(card); }

  short playCard() { return this->hand.getFirstCard(); }

  void collectCards(short c1, short c2) {
    if (c1 < c2) {
      this->hand.addCard(c2);
      this->hand.addCard(c1);
    } else {
      this->hand.addCard(c1);
      this->hand.addCard(c2);
    }
  }

  unsigned long handValue() const { return this->hand.handValue(); }
};

std::ostream& operator<<(std::ostream& output, const Player& p) {
  Hand hand = p.getHand();
  std::deque<short> cards = hand.getCards();
  unsigned int id = p.getId();
  output << "Player " << id << "'s deck: ";
  for (auto card : cards) output << card << ", ";
  return output;
}

class Game {
 private:
  Player player1;
  Player player2;

 public:
  Game(std::pair<Player, Player> players) : player1(players.first), player2(players.second) {}

  void play() {
    unsigned int round = 0;
    while (!player1.hasLost() && !player2.hasLost()) {
      ++round;
      short card1 = player1.playCard();
      short card2 = player2.playCard();
      if (card1 < card2) {
        player2.collectCards(card1, card2);
      } else {
        player1.collectCards(card1, card2);
      }
      std::cout << "-- Round " << round << " --" << std::endl;
      std::cout << player1 << std::endl;
      std::cout << player2 << std::endl;
      std::cout << std::endl;
    }
  }

  unsigned long getWinnersValue() const {
    if (player1.hasLost()) return player2.handValue();
    return player1.handValue();
  }
};

std::pair<Player, Player> parseInput(std::vector<std::string>& input) {
  auto it = input.begin();
  std::pair<Player, Player> players;
  for (unsigned int id = 1; id <= 2; ++id) {
    Player player(id);

    ++it;  // Read "Player <id>:"
    while (it != input.end() && *it != "") {
      player.addCard(std::atoi((*it).c_str()));
      ++it;
    }

    if (id == 1) {
      players.first = player;
    } else {
      players.second = player;
    }

    ++it;
  }
  return players;
}

void part1(std::pair<Player, Player> players) {
  Game game(players);
  game.play();
  std::cout << game.getWinnersValue() << std::endl;
}
void part2(std::pair<Player, Player> players) {}

int main() {
  const std::string filename = "../day-22/input.txt";
  std::vector<std::string> strInput = aoc::readStringInput(filename);
  std::pair players = parseInput(strInput);

  part1(players);
  part2(players);

  return 0;
}
