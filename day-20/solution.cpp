#include "aoclib.hpp"

unsigned int setBit(unsigned int value, size_t bit) {
  const std::array<unsigned int, 10> powersOfTwo = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512};
  return value | powersOfTwo[bit];
}

class Tile {
 private:
  const unsigned int id;
  std::vector<std::string> grid;
  std::vector<std::pair<unsigned int, unsigned int>> borders;
  std::array<unsigned int, 4> matches;
  unsigned int orientationIdx;  // Which idx is on top
  bool flipped;
  bool fixed;

 public:
  Tile(unsigned int id, std::vector<std::string> grid)
      : id(id), grid(grid), orientationIdx(0), flipped(false), fixed(false) {
    const size_t maxIdx = 9;

    // Top and bottom border
    unsigned int top1 = 0;
    unsigned int top2 = 0;
    unsigned int bottom1 = 0;
    unsigned int bottom2 = 0;
    for (size_t i = 0; i <= maxIdx; ++i) {
      if (grid[0][i] == '#') {
        top1 = setBit(top1, i);
        top2 = setBit(top2, maxIdx - i);
      }
      if (grid[maxIdx][i] == '#') {
        bottom1 = setBit(bottom1, i);
        bottom2 = setBit(bottom2, maxIdx - i);
      }
    }

    // Left and right border
    unsigned int left1 = 0;
    unsigned int left2 = 0;
    unsigned int right1 = 0;
    unsigned int right2 = 0;
    for (size_t i = 0; i <= maxIdx; ++i) {
      if (grid[i][0] == '#') {
        left1 = setBit(left1, i);
        left2 = setBit(left2, maxIdx - i);
      }
      if (grid[i][maxIdx] == '#') {
        right1 = setBit(right1, i);
        right2 = setBit(right2, maxIdx - i);
      }
    }

    this->borders = {{top1, top2}, {right1, right2}, {bottom1, bottom2}, {left1, left2}};
    this->matches = {0, 0, 0, 0};
  }

  unsigned int getId() const { return this->id; }
  std::vector<std::pair<unsigned int, unsigned int>> getBorders() const { return this->borders; }
  std::array<unsigned int, 4> getMatches() const { return this->matches; }
  bool isFixed() const { return this->fixed; }
  void setFixed() {
    if (this->fixed) {
      std::cerr << "Tile already fixed.";
      std::terminate();
    }
    this->fixed = true;
  }

  void checkMatch(Tile& other) {
    for (size_t i = 0; i < 4; ++i) {
      std::pair border = this->borders[i];
      for (size_t j = 0; j < 4; ++j) {
        std::pair borderOther = other.borders[j];
        if (border.first == borderOther.first || border.second == borderOther.first) {
          this->matches[i] = other.id;
          other.matches[j] = this->id;
        }
      }
    }
  }

  unsigned int getNumUnmatchedEdges() {
    unsigned int count = 0;
    for (auto m : this->matches) {
      if (m == 0) ++count;
    }
    return count;
  }

  bool isCorner() { return this->getNumUnmatchedEdges() == 2; }

  bool isEdge() { return this->getNumUnmatchedEdges() == 1; }

  void removeBorders() {
    const size_t maxIdx = 9;
    this->grid.erase(this->grid.begin() + maxIdx);
    this->grid.erase(this->grid.begin());
    for (size_t i = 0; i <= maxIdx - 2; ++i) {
      this->grid[i] = this->grid[i].substr(1, maxIdx - 1);
    }
  }

  void rotateLeft() {
    if (!this->flipped) {
      ++this->orientationIdx;
      this->orientationIdx %= 4;
    } else {
      this->orientationIdx = (this->orientationIdx + 3) % 4;
    }
  }

  void flip() { this->flipped = !this->flipped; }

  unsigned int getTileAbove() { return this->matches[this->orientationIdx]; }

  unsigned int getTileBelow() {
    unsigned int idx = (this->orientationIdx + 2) % 4;
    return this->matches[idx];
  }

  unsigned int getTileLeft() {
    unsigned int idx;
    if (!this->flipped) {
      idx = (this->orientationIdx + 3) % 4;
    } else {
      idx = (this->orientationIdx + 1) % 4;
    }
    return this->matches[idx];
  }

  unsigned int getTileRight() {
    unsigned int idx;
    if (!this->flipped) {
      idx = (this->orientationIdx + 1) % 4;
    } else {
      idx = (this->orientationIdx + 3) % 4;
    }
    return this->matches[idx];
  }

  static std::vector<std::string> rotateImageLeft(std::vector<std::string>& image) {
    std::vector<std::string> newImage(image[0].size(), "");
    for (size_t i = 0; i < image.size(); ++i) {
      for (size_t j = 0; j < image[0].size(); ++j) {
        std::string cell(1, image[i][j]);
        newImage[newImage.size() - 1 - j].append(cell);
      }
    }
    return newImage;
  }

  static std::vector<std::string> flipImage(std::vector<std::string> image) {
    // Intentionally passed by value!
    for (size_t i = 0; i < image.size(); ++i) {
      std::reverse(std::begin(image[i]), std::end(image[i]));
    }
    return image;
  }

  std::vector<std::string> getImage() {
    std::vector<std::string> image = this->grid;
    if (this->flipped) {
      image = this->flipImage(image);
      for (size_t i = 0; i < (4 - this->orientationIdx) % 4; ++i) {
        image = this->rotateImageLeft(image);
      }
    } else {
      for (size_t i = 0; i < this->orientationIdx; ++i) {
        image = this->rotateImageLeft(image);
      }
    }

    return image;
  }

  void print() {
    std::vector<std::string> image = this->getImage();
    std::cout << this->id << " (" << this->flipped << ", " << this->orientationIdx << ")" << std::endl;
    for (auto& s : image) {
      std::cout << s << std::endl;
    }
  }
};

std::ostream& operator<<(std::ostream& output, const Tile& t) {
  output << t.getId() << std::endl;
  auto borders = t.getBorders();
  auto matches = t.getMatches();
  for (size_t i = 0; i < 4; ++i) {
    auto border = borders[i];
    output << "\t" << border.first << ", " << border.second << " (" << matches[i] << ")" << std::endl;
  }
  return output;
}

std::unordered_map<unsigned int, std::unique_ptr<Tile>> createTiles(std::vector<std::string>& input) {
  std::unordered_map<unsigned int, std::unique_ptr<Tile>> tiles;

  auto it = input.begin();
  while (it != input.end() && *it != "") {
    std::string firstLine = *it;
    const unsigned int id = std::stoul(firstLine.substr(5, 4));

    ++it;
    auto gridBegin = it;
    while (*it != "") {
      ++it;
    }
    auto gridEnd = it;
    tiles[id] = std::make_unique<Tile>(id, std::vector(gridBegin, gridEnd));
    if (it != input.end()) ++it;
  }

  return tiles;
}

std::vector<std::string> constructImage(std::unordered_map<unsigned int, std::unique_ptr<Tile>>& tiles) {
  const size_t maxIdx = 9;
  int numTilesPerSide = sqrt(tiles.size());
  std::vector<std::vector<unsigned int>> idPositions(numTilesPerSide, std::vector<unsigned int>(numTilesPerSide, 0));

  // Find corner to begin with
  auto it = tiles.begin();
  while (!(*(*it).second).isCorner()) {
    ++it;
  }

  Tile& firstCorner = *(*it).second;
  while (firstCorner.getTileAbove() != 0 || firstCorner.getTileLeft() != 0) {
    firstCorner.rotateLeft();
  }
  firstCorner.setFixed();
  idPositions[0][0] = firstCorner.getId();

  // Finish first row
  unsigned int searchingForId = firstCorner.getTileRight();
  unsigned int prevId = firstCorner.getId();
  for (size_t j = 1; j < idPositions[0].size(); ++j) {
    Tile& tile = *tiles[searchingForId];

    if (tile.isFixed()) continue;
    if (tile.getId() != searchingForId) continue;

    if (tile.isEdge()) {
      while (tile.getTileAbove() != 0) tile.rotateLeft();
      if (tile.getTileRight() == prevId) tile.flip();
    } else if (tile.isCorner()) {
      while (tile.getTileAbove() != 0 || tile.getTileRight() != 0) tile.rotateLeft();
      if (tile.getTileBelow() == prevId) tile.flip();
      while (tile.getTileAbove() != 0 || tile.getTileRight() != 0) tile.rotateLeft();
    }

    idPositions[0][j] = tile.getId();
    tile.setFixed();
    searchingForId = tile.getTileRight();
    prevId = tile.getId();
  }

  // Finish first column
  searchingForId = firstCorner.getTileBelow();
  prevId = firstCorner.getId();
  for (size_t i = 1; i < idPositions.size(); ++i) {
    Tile& tile = *tiles[searchingForId];

    if (tile.isFixed()) continue;
    if (tile.getId() != searchingForId) continue;

    if (tile.isEdge()) {
      while (tile.getTileLeft() != 0) tile.rotateLeft();
      if (tile.getTileBelow() == prevId) tile.flip();
      while (tile.getTileLeft() != 0) tile.rotateLeft();
    } else if (tile.isCorner()) {
      while (tile.getTileLeft() != 0 || tile.getTileBelow() != 0) tile.rotateLeft();
      if (tile.getTileRight() == prevId) tile.flip();
      while (tile.getTileLeft() != 0 || tile.getTileBelow() != 0) tile.rotateLeft();
    }

    idPositions[i][0] = tile.getId();
    tile.setFixed();
    searchingForId = tile.getTileBelow();
    prevId = tile.getId();
  }

  // Finish the rest
  for (size_t i = 1; i < idPositions.size(); ++i) {
    for (size_t j = 1; j < idPositions[0].size(); ++j) {
      unsigned int tileIdAbove = idPositions[i - 1][j];
      unsigned int tileIdLeft = idPositions[i][j - 1];
      unsigned int tileId = tiles[tileIdAbove]->getTileBelow();
      if (tileId != tiles[tileIdLeft]->getTileRight()) {
        std::cerr << "Inconsistency" << std::endl;
        std::terminate();
      }
      Tile& tile = *tiles[tileId];
      while (tile.getTileAbove() != tileIdAbove) tile.rotateLeft();
      if (tile.getTileLeft() != tileIdLeft) tile.flip();
      idPositions[i][j] = tile.getId();
      tile.setFixed();
    }
  }

  std::vector<std::string> image(numTilesPerSide * (maxIdx - 1), "");
  for (size_t i = 0; i < numTilesPerSide; ++i) {
    for (size_t j = 0; j < numTilesPerSide; ++j) {
      unsigned int id = idPositions[i][j];
      if (id == 0) continue;  // If table not complete yet
      auto subimage = tiles[id]->getImage();
      for (size_t k = 0; k < subimage.size(); ++k) {
        image[i * (maxIdx - 1) + k] += subimage[k];
      }
    }
  }

  return image;
}

std::vector<std::string> getMonster() {
  std::string line1 = "                  # ";
  std::string line2 = "#    ##    ##    ###";
  std::string line3 = " #  #  #  #  #  #   ";
  return {line1, line2, line3};
}

std::vector<std::vector<size_t>> getMarkedPositions(std::vector<std::string> image) {
  std::vector<std::vector<size_t>> vec(image.size(), std::vector<size_t>());
  for (size_t line = 0; line < image.size(); ++line) {
    for (size_t i = 0; i < image[line].size(); ++i) {
      if (image[line][i] == '#') {
        vec[line].push_back(i);
      }
    }
  }
  return vec;
}

bool checkIfMonsterAtPosition(std::vector<std::string>& image, size_t posI, size_t posJ,
                              std::vector<std::vector<size_t>>& markedPositions, unsigned int monsterWidth,
                              unsigned int monsterHeight, bool mark = false) {
  if (posI <= image.size() - monsterHeight && posJ <= image[0].size() - monsterWidth) {
    for (int i = 0; i < monsterHeight; ++i) {
      for (auto j : markedPositions[i]) {
        if (image[posI + i][posJ + j] != '#') {
          return false;
        } else if (mark) {
          image[posI + i][posJ + j] = 'O';
        }
      }
    }
    return true;
  }
  return false;
}

void markMonstersOneOrientation(std::vector<std::string>& image, std::vector<std::string>& monster) {
  std::vector<std::vector<size_t>> markedPositions = getMarkedPositions(monster);

  for (size_t i = 0; i < image.size(); ++i) {
    for (size_t j = 0; j < image[i].size(); ++j) {
      if (checkIfMonsterAtPosition(image, i, j, markedPositions, monster[0].size(), monster.size())) {
        checkIfMonsterAtPosition(image, i, j, markedPositions, monster[0].size(), monster.size(), true);
      }
    }
  }
}

void markMonsters(std::vector<std::string>& image) {
  std::vector<std::string> monster = getMonster();

  for (size_t i = 0; i < 4; ++i) {
    markMonstersOneOrientation(image, monster);
    monster = Tile::rotateImageLeft(monster);
  }

  monster = Tile::flipImage(monster);

  for (size_t i = 0; i < 4; ++i) {
    markMonstersOneOrientation(image, monster);
    monster = Tile::rotateImageLeft(monster);
  }
}

void part1(std::unordered_map<unsigned int, std::unique_ptr<Tile>>& tiles) {
  unsigned long product = 1;
  for (auto& tile : tiles) {
    if (tile.second->isCorner()) {
      product *= tile.first;
    }
  }

  std::cout << product << std::endl;
}

void part2(std::vector<std::string>& image) {
  unsigned int count = 0;
  for (std::string& line : image) {
    for (char c : line) {
      if (c == '#') ++count;
    }
  }
  std::cout << count << std::endl;
}

int main() {
  const std::string filename = "../day-20/input.txt";
  std::vector<std::string> input = aoc::readStringInput(filename);

  std::unordered_map<unsigned int, std::unique_ptr<Tile>> tiles = createTiles(input);

  // Match tiles
  for (auto& tile : tiles) {
    for (auto& otherTile : tiles) {
      if (tile.first == otherTile.first) continue;
      tile.second->checkMatch(*otherTile.second);
    }
  }

  part1(tiles);

  for (auto& t : tiles) {
    t.second->removeBorders();
  }

  std::vector<std::string> image = constructImage(tiles);
  markMonsters(image);

  part2(image);

  for (auto& line : image) {
    std::cout << line << std::endl;
  }

  return 0;
}
