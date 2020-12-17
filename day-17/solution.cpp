#include "aoclib.hpp"

struct HashMapKey {
  unsigned int x, y, z, w;

  bool operator==(const HashMapKey &other) const {
    return (this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w);
  }

  unsigned int operator[](std::size_t idx) {
    switch (idx) {
      case 0:
        return this->x;
      case 1:
        return this->y;
      case 2:
        return this->z;
      case 3:
        return this->w;
      default:
        std::cerr << "Index " << idx << " out of range" << std::endl;
        std::terminate();
    }
  }
};

struct HashMapEntry {
  bool active;
  unsigned int neighboursActive;
};

struct KeyHasher {
  std::size_t operator()(const HashMapKey &k) const {
    size_t res = 17;
    res = res * 31 + k.x;
    res = res * 31 + k.y;
    res = res * 31 + k.z;
    res = res * 31 + k.w;
    return res;
  }
};

class SparseMatrix {
 private:
  std::unordered_map<HashMapKey, HashMapEntry, KeyHasher> values;
  bool useW;  // Whether to use the 4th dimension or not

 public:
  SparseMatrix(std::vector<std::string> &input, const unsigned int offset, bool useW = false) {
    for (unsigned int y = 0; y < input.size(); ++y) {
      for (unsigned int x = 0; x < input[0].size(); ++x) {
        if (input[y][x] == '#') {
          this->values[{x + offset, y + offset, 0 + offset, 0 + offset}] = {true, 0};
        }
      }
    }
    this->useW = useW;
  }

  std::unordered_map<HashMapKey, HashMapEntry, KeyHasher> getValues() const { return this->values; }

  void addActiveNeighbour(HashMapKey &key) {
    auto search = this->values.find(key);
    if (search == this->values.end()) {
      this->values[key] = {false, 1};
    } else {
      ++this->values[key].neighboursActive;
    }
  }

  std::pair<unsigned int, unsigned int> getRange(std::size_t dimension) const {
    if (this->values.size() == 0) {
      std::cerr << "No cell active" << std::endl;
      std::terminate();
    }
    HashMapKey firstElementKey = (*this->values.begin()).first;
    unsigned int min = firstElementKey[dimension];
    unsigned int max = firstElementKey[dimension];
    for (auto &val : this->values) {
      HashMapKey key = val.first;
      if (key[dimension] < min) min = key[dimension];
      if (key[dimension] > max) max = key[dimension];
    }
    return std::pair(min, max);
  }

  void makeStep() {
    std::vector<std::pair<const HashMapKey, HashMapEntry>> activeCells;
    for (auto &cell : this->values) {
      if (!cell.second.active) continue;
      activeCells.push_back(cell);
    }

    // Count active neighbours
    unsigned int wLimit = 0;
    unsigned int sub = 0;
    if (this->useW) {
      wLimit = 2;
      sub = 1;
    }
    for (auto &active : activeCells) {
      auto [curX, curY, curZ, curW] = active.first;
      for (unsigned int xi = 0; xi <= 2; ++xi) {
        unsigned int x = curX + xi - 1;
        for (unsigned int yi = 0; yi <= 2; ++yi) {
          unsigned int y = curY + yi - 1;
          for (unsigned int zi = 0; zi <= 2; ++zi) {
            unsigned int z = curZ + zi - 1;
            for (unsigned int wi = 0; wi <= wLimit; ++wi) {
              unsigned int w = curW + wi - sub;
              HashMapKey key = {x, y, z, w};
              this->addActiveNeighbour(key);
            }
          }
        }
      }
      // Instead of another if inside the for loops:
      --this->values[active.first].neighboursActive;
    }

    // Change states & reset
    std::vector<HashMapKey> toBeErased;
    for (auto &cell : this->values) {
      unsigned int neighbours = cell.second.neighboursActive;
      if (cell.second.active && (neighbours < 2 || neighbours > 3)) {
        cell.second.active = false;
      } else if (!cell.second.active && neighbours == 3) {
        cell.second.active = true;
      }
      // Remove inactive & reset
      if (!cell.second.active) {
        toBeErased.push_back(cell.first);
      } else {
        cell.second.neighboursActive = 0;
      }
    }
    for (auto &key : toBeErased) {
      this->values.erase(key);
    }
  }

  unsigned int getNumActiveCells() { return this->values.size(); }
};

std::ostream &operator<<(std::ostream &output, const SparseMatrix &sm) {
  auto values = sm.getValues();
  auto [xMin, xMax] = sm.getRange(0);
  auto [yMin, yMax] = sm.getRange(1);
  auto [zMin, zMax] = sm.getRange(2);
  auto [wMin, wMax] = sm.getRange(3);

  for (unsigned int w = wMin; w <= wMax; ++w) {
    for (unsigned int z = zMin; z <= zMax; ++z) {
      output << "Layer z=" << z << ", w=" << w << std::endl;
      for (unsigned int y = yMin; y <= yMax; ++y) {
        for (unsigned int x = xMin; x <= xMax; ++x) {
          HashMapEntry &entry = values[{x, y, z, w}];
          if (entry.active) {
            output << "#";
          } else {
            output << ".";
          }
        }
        output << std::endl;
      }
    }
  }
  return output;
}

void solve(std::vector<std::string> &input, const unsigned int offset, bool useW = false) {
  SparseMatrix matrix = SparseMatrix(input, offset, useW);
  for (int i = 0; i < 6; ++i) {
    matrix.makeStep();
  }
  std::cout << matrix.getNumActiveCells() << std::endl;
}

int main() {
  const std::string filename = "../day-17/input.txt";
  auto input = aoc::readStringInput(filename);

  const unsigned int offset = 6;  // To avoid negative numbers for coordinates (hash calculation!)

  solve(input, offset);        // Star 1
  solve(input, offset, true);  // Star 2

  return 0;
}
