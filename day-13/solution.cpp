#include "aoclib.hpp"

std::vector<unsigned int> getBusIds(std::string& input, bool skipX = true) {
  std::vector<std::string> v;
  boost::split(v, input, boost::is_any_of(","));
  std::vector<unsigned int> busIds;
  for (auto& bus : v) {
    if (bus != "x") {
      busIds.push_back(std::stoul(bus));
    } else if (!skipX) {
      busIds.push_back(0);
    }
  }
  return busIds;
}

void part1(unsigned int timestamp, std::vector<unsigned int>& busIds) {
  unsigned int earliestBus = 0;
  unsigned int waitingTime = UINT_MAX;
  for (auto bus : busIds) {
    unsigned int wait = bus - (timestamp % bus);
    if (wait < waitingTime) {
      earliestBus = bus;
      waitingTime = wait;
    }
  }
  std::cout << earliestBus << " * " << waitingTime << " = " << (earliestBus * waitingTime) << std::endl;
}

void part2(std::vector<unsigned int>& busIds) {
  unsigned int numMatched = 1;
  uint64_t currentTime = busIds[0];
  uint64_t currentStep = busIds[0];
  while (numMatched < busIds.size()) {
    unsigned int currentlyMatching = numMatched;

    while (busIds[currentlyMatching] == 0) currentlyMatching++;
    while ((currentTime + currentlyMatching) % busIds[currentlyMatching] != 0) currentTime += currentStep;

    currentStep *= busIds[currentlyMatching];
    numMatched = currentlyMatching + 1;
  }
  std::cout << currentTime << std::endl;
}

int main() {
  const std::string filename = "../day-13/input.txt";
  auto input = aoc::readStringInput(filename);

  unsigned int timestamp = std::stoul(input[0]);
  std::vector<unsigned int> busIds = getBusIds(input[1]);
  part1(timestamp, busIds);

  std::vector<unsigned int> allBusIds = getBusIds(input[1], false);
  part2(allBusIds);

  return 0;
}
