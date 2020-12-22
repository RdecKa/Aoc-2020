#include <map>

#include "aoclib.hpp"

class Hand {
 private:
  std::deque<short> cards;
  Hand(std::deque<short>& cards) : cards(cards) {}

 public:
  Hand() : cards(std::deque<short>()){};

  std::deque<short> getCards() const { return this->cards; }
  short getNumberOfCards() const { return this->cards.size(); }
  bool empty() const { return this->cards.empty(); }

  void addCard(short card) { this->cards.push_back(card); }

  short getFirstCard() {
    short card = this->cards.front();
    this->cards.pop_front();
    return card;
  }

  // Returns a copy of hand with only the first n cards
  Hand getSubHand(short n) const {
    if (n > this->cards.size()) n = this->cards.size();
    auto newCards = std::deque<short>(this->cards.begin(), this->cards.begin() + n);
    return Hand(newCards);
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

bool operator<(const Hand& lhs, const Hand& rhs) { return lhs.getCards() < rhs.getCards(); }

class Player {
 private:
  unsigned int id;
  Hand hand;

  void setHand(Hand& hand) { this->hand = hand; }

 public:
  Player() = default;
  Player(unsigned int id) : id(id), hand(Hand()) {}

  unsigned int getId() const { return this->id; }
  Hand getHand() const { return this->hand; }
  short getNumberOfCards() const { return this->hand.getNumberOfCards(); }
  bool hasLost() const { return this->hand.empty(); }

  void addCard(short card) { this->hand.addCard(card); }

  short playCard() { return this->hand.getFirstCard(); }

  void collectCards(short cWinner, short cLoser) {
    this->hand.addCard(cWinner);
    this->hand.addCard(cLoser);
  }

  unsigned long handValue() const { return this->hand.handValue(); }

  Player getSubCopy(short n) const {
    Player newPlayer(this->id);
    Hand newHand = this->hand.getSubHand(n);
    newPlayer.setHand(newHand);
    return newPlayer;
  }
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
 protected:
  Player player1;
  Player player2;

 public:
  Game(std::pair<Player, Player>& players) : player1(players.first), player2(players.second) {}

  void play() {
    unsigned int round = 0;
    while (!this->player1.hasLost() && !this->player2.hasLost()) {
      ++round;
      short card1 = this->player1.playCard();
      short card2 = this->player2.playCard();
      if (card1 < card2) {
        this->player2.collectCards(card2, card1);
      } else {
        this->player1.collectCards(card1, card2);
      }
    }
  }

  unsigned long getWinnersValue() const {
    if (player1.hasLost()) return player2.handValue();
    return player1.handValue();
  }
};

class RecursiveGame : public Game {
 private:
  unsigned int gameLevel;
  // Key is the number of cards of 1st player
  std::map<short, std::set<std::pair<Hand, Hand>>> previousConfigurations;

  bool findOrInsertConfiguration(const Hand& hand1, const Hand& hand2) {
    std::pair<Hand, Hand> hands = std::make_pair(hand1, hand2);
    short mapKey = hand1.getNumberOfCards();

    auto it = this->previousConfigurations.find(mapKey);

    if (it == this->previousConfigurations.end()) {
      // Definitely not seen yet
      std::set<std::pair<Hand, Hand>> newValue = {hands};
      this->previousConfigurations.insert(std::make_pair(mapKey, newValue));
      return true;
    }

    auto [iter, inserted] = (*it).second.insert(hands);
    return inserted;
  }

  // Returns true if configuration was found among the previous configurations
  bool checkIfConfigurationSeen(const Hand& hand1, const Hand& hand2) {
    return !this->findOrInsertConfiguration(hand1, hand2);
  }

  // Returns ID of the winner
  unsigned int playSubGame() {
    unsigned int round = 0;
    while (!this->player1.hasLost() && !this->player2.hasLost()) {
      ++round;

      if (this->checkIfConfigurationSeen(this->player1.getHand(), this->player2.getHand())) {
        return 1;
      }

      short card1 = this->player1.playCard();
      short card2 = this->player2.playCard();

      unsigned int winner;

      if (player1.getNumberOfCards() >= card1 && player2.getNumberOfCards() >= card2) {
        Player newPlayer1 = this->player1.getSubCopy(card1);
        Player newPlayer2 = this->player2.getSubCopy(card2);
        RecursiveGame newGame(std::make_pair(newPlayer1, newPlayer2), this->gameLevel + 1);
        winner = newGame.playSubGame();
      } else if (card1 < card2) {
        winner = 2;
      } else {
        winner = 1;
      }

      switch (winner) {
        case 1:
          this->player1.collectCards(card1, card2);
          break;
        case 2:
          this->player2.collectCards(card2, card1);
          break;
        default:
          std::cerr << "Invalid winner ID " << winner << std::endl;
          std::terminate();
      }
    }
    if (this->player1.hasLost()) return 2;
    return 1;
  }

 public:
  RecursiveGame(std::pair<Player, Player> players, unsigned int gameLevel = 1) : Game(players), gameLevel(gameLevel) {}

  void play() { this->playSubGame(); }
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

void part2(std::pair<Player, Player> players) {
  RecursiveGame game(players);
  game.play();
  std::cout << game.getWinnersValue() << std::endl;
}

int main() {
  const std::string filename = "../day-22/input.txt";
  std::vector<std::string> strInput = aoc::readStringInput(filename);
  std::pair players = parseInput(strInput);

  part1(players);
  part2(players);

  return 0;
}
