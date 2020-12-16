#include "aoclib.hpp"

/*
 * Field
 */

class Field {
 private:
  std::string name;
  std::pair<unsigned int, unsigned int> range1;
  std::pair<unsigned int, unsigned int> range2;

 public:
  Field(std::string& name, unsigned int minRange1, unsigned int maxRange1, unsigned int minRange2,
        unsigned int maxRange2)
      : name(name), range1(minRange1, maxRange1), range2(minRange2, maxRange2) {}

  std::string getName() const { return name; }
  std::pair<unsigned int, unsigned int> getRange1() const { return range1; }
  std::pair<unsigned int, unsigned int> getRange2() const { return range2; }

  bool isValueValid(unsigned int value) {
    return this->range1.first <= value && this->range1.second >= value ||
           this->range2.first <= value && this->range2.second >= value;
  }
};

std::ostream& operator<<(std::ostream& output, const Field& f) {
  auto range1 = f.getRange1();
  auto range2 = f.getRange2();
  return output << f.getName() << " (" << range1.first << ", " << range1.second << "), (" << range2.first << ", "
                << range2.second << ")";
}

/*
 * Ticket
 */

class Ticket {
 private:
  std::vector<unsigned int> values;

 public:
  Ticket(const std::vector<std::string>& stringValues) {
    this->values = std::vector<unsigned int>(stringValues.size());
    for (int i = 0; i < stringValues.size(); ++i) {
      this->values[i] = std::stoul(stringValues[i]);
    }
  }

  std::vector<unsigned int> getValues() const { return values; }
  unsigned int getValue(unsigned int idx) const { return values[idx]; }
};

std::ostream& operator<<(std::ostream& output, const Ticket& t) {
  for (auto val : t.getValues()) output << val << " ";
  return output;
}

/*
 * Solver (star 2)
 */
class Solver {
 private:
  std::vector<std::vector<Field*>> possibilities;
  const std::vector<Ticket> tickets;
  std::vector<Field*> fields;
  std::vector<bool> fixed;

 public:
  Solver(std::vector<Field>& fields, const std::vector<Ticket>& tickets) : tickets(tickets) {
    this->possibilities = std::vector(fields.size(), std::vector<Field*>());
    for (auto& field : fields) {
      for (auto& pos : this->possibilities) {
        pos.push_back(&field);
      }
      this->fields.push_back(&field);
    }
    this->fixed = std::vector<bool>(this->possibilities.size(), false);

    // Remove options where values out of range
    this->removeImpossible();

    bool changed1 = true;
    bool changed2 = true;
    while (changed1 || changed2) {
      changed1 = this->fixPositionsWithOneOption();
      if (changed1) this->removeFixed();
      changed2 = this->fixFieldsWithOnlyOneOption();
      if (changed2) this->removeFixed();
    }
  }

  std::vector<std::vector<Field*>> getPossibilities() const { return possibilities; }

  void removeImpossible() {
    for (int fieldIdx = 0; fieldIdx < this->possibilities.size(); ++fieldIdx) {
      std::vector<Field*>& fieldCandidates = this->possibilities[fieldIdx];
      std::vector<int> unsuitableCandidatesIdxs;
      for (int candidateIdx = 0; candidateIdx < fieldCandidates.size(); ++candidateIdx) {
        for (auto& ticket : this->tickets) {
          if (!fieldCandidates[candidateIdx]->isValueValid(ticket.getValue(fieldIdx))) {
            unsuitableCandidatesIdxs.push_back(candidateIdx);
            break;
          }
        }
      }
      for (int i = unsuitableCandidatesIdxs.size() - 1; i >= 0; --i) {
        // Start from back because removing elements from the vector!
        unsigned int candidateIdx = unsuitableCandidatesIdxs[i];
        fieldCandidates.erase(fieldCandidates.begin() + candidateIdx);
      }
    }
  }

  bool fixPositionsWithOneOption() {
    bool change = false;
    for (int i = 0; i < this->possibilities.size(); ++i) {
      if (this->fixed[i]) continue;
      auto& possibility = this->possibilities[i];
      if (possibility.size() == 1) {
        this->fixed[i] = true;
        change = true;
      }
    }
    return change;
  }

  bool fixFieldsWithOnlyOneOption() {
    bool change = false;
    for (auto field : this->fields) {
      int count = 0;
      int foundAt = -1;
      for (int i = 0; i < this->possibilities.size(); ++i) {
        auto& pos = this->possibilities[i];
        auto search = std::find(pos.begin(), pos.end(), field);
        if (search != pos.end()) {
          ++count;
          foundAt = i;
          if (count > 1) break;
        }
      }
      if (count == 1 && !this->fixed[foundAt]) {
        this->fixed[foundAt] = true;
        this->possibilities[foundAt].clear();
        this->possibilities[foundAt].push_back(field);
        change = true;
      }
    }
    return change;
  }

  void removeFixed() {
    for (int i = 0; i < this->fixed.size(); ++i) {
      if (this->fixed[i]) {
        auto field = this->possibilities[i][0];
        for (int j = 0; j < this->possibilities.size(); ++j) {
          if (!this->fixed[j]) {
            auto& pos = this->possibilities[j];
            auto search = std::find(pos.begin(), pos.end(), field);
            if (search != pos.end()) {
              pos.erase(search);
            }
          }
        }
      }
    }
  }

  unsigned long calculateStar2() {
    Ticket myTicket = this->tickets[this->tickets.size() - 1];
    unsigned long result = 1;
    for (int i = 0; i < this->possibilities.size(); ++i) {
      auto& f = *this->possibilities[i][0];
      if (f.getName()[0] == 'd' && f.getName()[1] == 'e') {
        result *= static_cast<unsigned long>(myTicket.getValue(i));
      }
    }
    return result;
  }
};

std::ostream& operator<<(std::ostream& output, const Solver& s) {
  std::vector<std::vector<Field*>> possibilities = s.getPossibilities();
  for (int i = 0; i < possibilities.size(); ++i) {
    output << i << ": ";
    std::vector<Field*> vectorOfPossibleFields = possibilities[i];
    for (auto& f : vectorOfPossibleFields) {
      output << f->getName() << " ";
    }
    output << std::endl;
  }
  return output;
}

/*
 * Parse input
 */

std::tuple<std::vector<Field>, Ticket, std::vector<Ticket>> parseInput(const std::string& filename) {
  std::fstream file;
  file.open(filename, std::ios::in);

  if (file.is_open()) {
    std::string line;
    std::vector<std::string> splitValues;

    // Read fields
    std::vector<Field> fields;
    std::string fieldName;
    while (std::getline(file, line) && line.size() > 0) {
      // Split in two parts
      boost::split(splitValues, line, boost::is_any_of(":"));
      fieldName = splitValues[0];

      // Split second part further
      boost::split(splitValues, splitValues[1], boost::is_any_of(" -"));
      fields.push_back(Field(fieldName, std::stoul(splitValues[1]), std::stoul(splitValues[2]),
                             std::stoul(splitValues[4]), std::stoul(splitValues[5])));
      splitValues.clear();
    }

    // Read my ticket
    std::getline(file, line);  // Read "your ticket"
    std::getline(file, line);  // Read values on my ticket
    boost::split(splitValues, line, boost::is_any_of(","));
    Ticket myTicket = Ticket(splitValues);

    // Read nearby tickets
    std::vector<Ticket> nearbyTickets;
    std::getline(file, line);  // Read empty line
    std::getline(file, line);  // Read "nearby ticket"
    while (std::getline(file, line) && line.size() > 0) {
      boost::split(splitValues, line, boost::is_any_of(","));
      fieldName = splitValues[0];
      nearbyTickets.push_back(Ticket(splitValues));
      splitValues.clear();
    }

    return std::tuple(fields, myTicket, nearbyTickets);
  } else {
    std::cerr << "Can't open file " << filename << std::endl;
    std::cerr << "PWD: " << std::filesystem::current_path() << std::endl;
    std::terminate();
  }
}

/*
 * Solve star 1
 */

unsigned int getMaxPossibleValue(const std::vector<Field>& fields) {
  unsigned int max = 0;
  for (auto& field : fields) {
    unsigned int endRange2 = field.getRange2().second;
    if (endRange2 > max) max = endRange2;
  }
  return max;
}

std::vector<bool> mergeValidRanges(const std::vector<Field>& fields) {
  unsigned int maxValue = getMaxPossibleValue(fields);
  std::vector<bool> validValues(maxValue + 1, false);
  for (auto& field : fields) {
    std::pair range1 = field.getRange1();
    for (unsigned int i = range1.first; i <= range1.second; ++i) {
      validValues[i] = true;
    }
    std::pair range2 = field.getRange2();
    for (unsigned int i = range2.first; i <= range2.second; ++i) {
      validValues[i] = true;
    }
  }
  return validValues;
}

std::vector<Ticket> part1(const std::vector<Field>& fields, const std::vector<Ticket>& nearbyTickets) {
  std::vector validValues = mergeValidRanges(fields);
  std::vector<Ticket> validTickets;
  unsigned int sum = 0;
  for (auto& ticket : nearbyTickets) {
    bool errorFound = false;
    for (auto val : ticket.getValues()) {
      if (val >= validValues.size() or !validValues[val]) {
        sum += val;
        errorFound = true;
      }
    }
    if (!errorFound) validTickets.push_back(ticket);
  }
  std::cout << sum << std::endl;
  return validTickets;
}

/*
 * Solve star 2
 */
std::vector<Ticket> getValidTickets(const std::vector<Field>& fields, const std::vector<Ticket>& tickets) {
  std::vector<Ticket> validTickets;
  std::vector validValues = mergeValidRanges(fields);
  for (auto& ticket : tickets) {
    bool errorFound = false;
    for (auto val : ticket.getValues()) {
      if (val >= validValues.size() or !validValues[val]) {
        errorFound = true;
      }
    }
    if (!errorFound) validTickets.push_back(ticket);
  }
  return validTickets;
}

void part2(std::vector<Field>& fields, const std::vector<Ticket>& tickets) {
  Solver solver(fields, tickets);
  // std::cout << solver;
  std::cout << solver.calculateStar2() << std::endl;
}

int main() {
  const std::string filename = "../day-16/input.txt";
  auto input = parseInput(filename);

  std::vector<Field> fields = std::get<0>(input);
  Ticket myTicket = std::get<1>(input);
  std::vector<Ticket> nearbyTickets = std::get<2>(input);

  std::vector<Ticket> validTickets = part1(fields, nearbyTickets);
  validTickets.push_back(myTicket);

  part2(fields, validTickets);

  return 0;
}
