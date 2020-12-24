#include <map>

#include "aoclib.hpp"

enum class Direction { E, SE, SW, W, NW, NE };

struct Path {
  std::vector<Direction> path;
};

Path parseInput(std::string& line) {
  auto it = line.begin();
  std::vector<Direction> path;
  while (it != line.end()) {
    switch (*it) {
      case 'e':
        path.push_back(Direction::E);
        break;
      case 'w':
        path.push_back(Direction::W);
        break;
      case 's':
        switch (*(it + 1)) {
          case 'w':
            path.push_back(Direction::SW);
            break;
          case 'e':
            path.push_back(Direction::SE);
            break;
          default:
            std::terminate();
        }
        ++it;
        break;
      case 'n':
        switch (*(it + 1)) {
          case 'w':
            path.push_back(Direction::NW);
            break;
          case 'e':
            path.push_back(Direction::NE);
            break;
          default:
            std::terminate();
        }
        ++it;
        break;
      default:
        std::terminate();
    }
    ++it;
  }

  return {path};
}

class TileFloor {
 private:
  // If coordinate in the set, the tile is black
  // Key is a coordinate (axial coordinates)
  std::set<std::pair<short, short>> blackTiles;

  std::pair<short, short> getCoordinates(Path& path) {
    short q = 0;
    short r = 0;
    for (Direction d : path.path) {
      switch (d) {
        case Direction::E:
          ++q;
          break;
        case Direction::SE:
          ++r;
          break;
        case Direction::SW:
          --q;
          ++r;
          break;
        case Direction::W:
          --q;
          break;
        case Direction::NW:
          --r;
          break;
        case Direction::NE:
          ++q;
          --r;
          break;
        default:
          std::terminate();
      }
    }
    return {q, r};
  }

 public:
  TileFloor() : blackTiles(std::set<std::pair<short, short>>()) {}

  void flipTile(Path& path) {
    std::pair<short, short> coords = this->getCoordinates(path);
    auto [it, inserted] = this->blackTiles.insert(coords);
    if (!inserted) {
      // Tile already black
      this->blackTiles.erase(coords);
    }
  }

  size_t countBlackTiles() const { return this->blackTiles.size(); }

  void makeChangeOfOneDay() {
    std::map<std::pair<short, short>, short> numBlackNeighbours;
    const std::array<short, 6> deltaR = {0, 1, 1, 0, -1, -1};
    const std::array<short, 6> deltaQ = {1, 0, -1, -1, 0, 1};

    for (auto& tile : this->blackTiles) {
      short q = tile.first;
      short r = tile.second;
      for (size_t i = 0; i < deltaR.size(); ++i) {
        std::pair<short, short> coords = {q + deltaQ[i], r + deltaR[i]};
        auto search = numBlackNeighbours.find(coords);
        if (search == numBlackNeighbours.end()) {
          numBlackNeighbours[coords] = 1;
        } else {
          ++numBlackNeighbours[coords];
        }
      }
    }

    // Add black tiles with no black neighbours
    for (auto& tile : this->blackTiles) {
      auto search = numBlackNeighbours.find(tile);
      if (search == numBlackNeighbours.end()) {
        numBlackNeighbours[tile] = 0;
      }
    }

    // Flip tiles
    for (auto& tile : numBlackNeighbours) {
      std::pair<short, short> coords = tile.first;
      short numNeighbours = tile.second;

      bool isBlack = true;
      auto search = this->blackTiles.find(coords);
      if (search == this->blackTiles.end()) isBlack = false;

      if (isBlack && (numNeighbours == 0 || numNeighbours > 2)) {
        this->blackTiles.erase(coords);
      } else if (!isBlack && numNeighbours == 2) {
        this->blackTiles.insert(coords);
      }
    }
  }
};

void part1(std::vector<Path>& input) {
  TileFloor floor;
  for (Path& path : input) {
    floor.flipTile(path);
  }
  std::cout << floor.countBlackTiles() << std::endl;
}

void part2(std::vector<Path>& input) {
  TileFloor floor;
  for (Path& path : input) {
    floor.flipTile(path);
  }
  for (int i = 0; i < 100; ++i) {
    floor.makeChangeOfOneDay();
  }
  std::cout << floor.countBlackTiles() << std::endl;
}

int main() {
  const std::string filename = "../day-24/input.txt";
  std::vector<Path> input = aoc::readParseInput(filename, parseInput);

  part1(input);
  part2(input);

  return 0;
}
