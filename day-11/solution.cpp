#include "aoclib.hpp"

enum State { floor, empty, occupied };

std::ostream& operator<<(std::ostream& output, const State& s) {
  switch (s) {
    case floor:
      return output << ".";
    case empty:
      return output << "L";
    case occupied:
      return output << "#";
    default:
      return output << "?";
  }
}

std::vector<State> parseInput(std::string& line) {
  auto row = std::vector<State>(line.size(), empty);
  for (int i = 0; i < line.size(); ++i) {
    if (line[i] == '.') row[i] = floor;
  }
  return row;
}

int makeChanges(std::vector<std::vector<State>>& grid, std::vector<std::vector<unsigned short>>& helperGrid,
                int threshold) {
  int numChanges = 0;
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[i].size(); ++j) {
      if (grid[i][j] == empty && helperGrid[i][j] == 0) {
        grid[i][j] = occupied;
        ++numChanges;
      } else if (grid[i][j] == occupied && helperGrid[i][j] >= threshold) {
        grid[i][j] = empty;
        ++numChanges;
      }
      // Get ready for next iteration
      helperGrid[i][j] = 0;
    }
  }
  return numChanges;
}

// Make sure that helperGrid contains only zeros
// Returns number of seat changes
int simulateNextStep(std::vector<std::vector<State>>& grid, std::vector<std::vector<unsigned short>>& helperGrid) {
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[0].size(); ++j) {
      if (grid[i][j] == occupied) {
        if (i > 0) {
          ++helperGrid[i - 1][j];
          if (j > 0) ++helperGrid[i - 1][j - 1];
          if (j < grid[i].size() - 1) ++helperGrid[i - 1][j + 1];
        }
        if (i < grid.size() - 1) {
          ++helperGrid[i + 1][j];
          if (j > 0) ++helperGrid[i + 1][j - 1];
          if (j < grid[i].size() - 1) ++helperGrid[i + 1][j + 1];
        }
        if (j > 0) ++helperGrid[i][j - 1];
        if (j < grid[i].size()) ++helperGrid[i][j + 1];
      }
    }
  }

  return makeChanges(grid, helperGrid, 4);
}

// Make sure that helperGrid contains only zeros
// Returns number of seat changes
int simulateNextStep2(std::vector<std::vector<State>>& grid, std::vector<std::vector<unsigned short>>& helperGrid) {
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[0].size(); ++j) {
      if (grid[i][j] == occupied) {
        // Left
        int k = 1;
        while (j - k >= 0 && grid[i][j - k] == floor) ++k;
        if (j - k >= 0) ++helperGrid[i][j - k];

        // Right
        k = 1;
        while (j + k < grid[i].size() && grid[i][j + k] == floor) ++k;
        if (j + k < grid[i].size()) ++helperGrid[i][j + k];

        // Up
        k = 1;
        while (i - k >= 0 && grid[i - k][j] == floor) ++k;
        if (i - k >= 0) ++helperGrid[i - k][j];

        // Down
        k = 1;
        while (i + k < grid.size() && grid[i + k][j] == floor) ++k;
        if (i + k < grid.size()) ++helperGrid[i + k][j];

        // Up left
        k = 1;
        while (i - k >= 0 && j - k >= 0 && grid[i - k][j - k] == floor) ++k;
        if (i - k >= 0 && j - k >= 0) ++helperGrid[i - k][j - k];

        // Down left
        k = 1;
        while (i + k < grid.size() && j - k >= 0 && grid[i + k][j - k] == floor) ++k;
        if (i + k < grid.size() && j - k >= 0) ++helperGrid[i + k][j - k];

        // Up right
        k = 1;
        while (i - k >= 0 && j + k < grid[i].size() && grid[i - k][j + k] == floor) ++k;
        if (i - k >= 0 && j + k < grid[i].size()) ++helperGrid[i - k][j + k];

        // Down right
        k = 1;
        while (i + k < grid.size() && j + k < grid[i].size() && grid[i + k][j + k] == floor) ++k;
        if (i + k < grid.size() && j + k < grid[i].size()) ++helperGrid[i + k][j + k];
      }
    }
  }

  return makeChanges(grid, helperGrid, 5);
}

int countOccupiedSeats(std::vector<std::vector<State>>& grid) {
  int numOccupiedSeats = 0;
  for (auto row : grid) {
    for (auto seat : row) {
      if (seat == occupied) ++numOccupiedSeats;
    }
  }
  return numOccupiedSeats;
}

void part1(std::vector<std::vector<State>>& input) {
  int numChanges = -1;
  auto helperGrid = std::vector<std::vector<unsigned short>>(input.size());
  for (int i = 0; i < input.size(); ++i) {
    helperGrid[i] = std::vector<unsigned short>(input[i].size(), 0);
  }
  while (numChanges != 0) {
    numChanges = simulateNextStep(input, helperGrid);
  }
  int result = countOccupiedSeats(input);
  std::cout << result << std::endl;
}

void part2(std::vector<std::vector<State>>& input) {
  int numChanges = -1;
  auto helperGrid = std::vector<std::vector<unsigned short>>(input.size());
  for (int i = 0; i < input.size(); ++i) {
    helperGrid[i] = std::vector<unsigned short>(input[i].size(), 0);
  }
  while (numChanges != 0) {
    numChanges = simulateNextStep2(input, helperGrid);
  }
  int result = countOccupiedSeats(input);
  std::cout << result << std::endl;
}

int main() {
  const std::string filename = "../day-11/input.txt";
  auto input = aoc::readParseInput(filename, parseInput);
  std::vector<std::vector<State>> input2(input);

  part1(input);
  part2(input2);

  return 0;
}
