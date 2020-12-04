#include "aoclib.hpp"

class Passport {
 private:
  std::optional<int> byr;
  std::optional<int> iyr;
  std::optional<int> eyr;
  std::optional<std::string> hgt;
  std::optional<std::string> hcl;
  std::optional<std::string> ecl;
  std::optional<std::string> pid;
  std::optional<std::string> cid;

 public:
  Passport(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    auto it = begin;
    while (it != end) {
      std::vector<std::string> v;
      boost::split(v, *it, boost::is_any_of(" :"));

      for (int i = 0; i < v.size(); i += 2) {
        if (v[i] == "byr") {
          this->byr = std::atoi(v[i + 1].c_str());
        } else if (v[i] == "iyr") {
          this->iyr = std::atoi(v[i + 1].c_str());
        } else if (v[i] == "eyr") {
          this->eyr = std::atoi(v[i + 1].c_str());
        } else if (v[i] == "hgt") {
          this->hgt = v[i + 1];
        } else if (v[i] == "hcl") {
          this->hcl = v[i + 1];
        } else if (v[i] == "ecl") {
          this->ecl = v[i + 1];
        } else if (v[i] == "pid") {
          this->pid = v[i + 1];
        } else if (v[i] == "cid") {
          this->cid = v[i + 1];
        }
      }

      ++it;
    }
  }

  bool isValid() const {
    return this->byr.has_value() && this->iyr.has_value() && this->eyr.has_value() && this->hgt.has_value() &&
           this->hcl.has_value() && this->ecl.has_value() && this->pid.has_value();
  }

  bool isReallyValid() const {
    if (!this->isValid()) return false;

    if (this->byr.value() < 1920 || this->byr.value() > 2002) return false;
    if (this->iyr.value() < 2010 || this->iyr.value() > 2020) return false;
    if (this->eyr.value() < 2020 || this->eyr.value() > 2030) return false;

    auto hgt = this->hgt.value();
    if (hgt.length() < 3) return false;
    auto unit = hgt.substr(hgt.length() - 2, 2);
    if (unit != "cm" && unit != "in") return false;
    auto h = std::atoi(hgt.substr(0, hgt.length() - 2).c_str());
    if (unit == "cm" && (h < 150 || h > 193)) return false;
    if (unit == "in" && (h < 59 || h > 76)) return false;

    auto hcl = this->hcl.value();
    if (hcl.length() != 7) return false;
    if (hcl[0] != '#') return false;
    for (int i = 1; i < 7; ++i) {
      if (!(hcl[i] >= '0' && hcl[i] <= '9' || hcl[i] >= 'a' && hcl[i] <= 'f')) return false;
    }

    std::set<std::string> validColors = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
    if (validColors.find(this->ecl.value()) == validColors.end()) return false;

    auto pid = this->pid.value();
    if (pid.length() != 9) return false;
    for (int i = 0; i < 9; ++i) {
      if (pid[i] < '0' || pid[i] > '9') return false;
    }

    return true;
  }
};

std::vector<Passport> createDatabase(std::vector<std::string> input) {
  auto v = std::vector<Passport>();
  auto it = input.begin();
  while (it != input.end()) {
    auto begin = it;
    while ((*it).length() > 0 && it != input.end()) {
      ++it;
    }
    auto p = Passport(begin, it);
    v.push_back(p);
    if (it != input.end()) {
      ++it;
    }
  }
  return v;
}

void part1(std::vector<Passport> input) {
  int count = 0;
  for (auto p : input) {
    if (p.isValid()) {
      ++count;
    }
  }
  std::cout << count << "/" << input.size() << " passports valid" << std::endl;
}

void part2(std::vector<Passport> input) {
  int count = 0;
  for (auto p : input) {
    if (p.isReallyValid()) {
      ++count;
    }
  }
  std::cout << count << "/" << input.size() << " passports really valid" << std::endl;
}

int main() {
  const std::string filename = "../day-04/input.txt";
  auto parsed_input = aoc::readStringInput(filename);
  auto db = createDatabase(parsed_input);

  part1(db);
  part2(db);

  return 0;
}
