#include "aoclib.hpp"

unsigned long getLoopSize(unsigned long publicKey) {
  unsigned long value = 1;
  unsigned long loopSize = 0;
  const unsigned long subjectNumber = 7;
  const unsigned long mod = 20201227;

  while (value != publicKey) {
    value = (value * subjectNumber) % mod;
    ++loopSize;
  }

  return loopSize;
}

unsigned long getEncryptionKey(unsigned long loopSize, unsigned long subjectNumber) {
  unsigned long value = 1;
  const unsigned long mod = 20201227;

  for (size_t i = 0; i < loopSize; ++i) {
    value = (value * subjectNumber) % mod;
  }
  return value;
}

int main() {
  // My input
  unsigned long cardPublic = 10441485;
  unsigned long doorPublic = 1004920;

  // Test input
  // unsigned long cardPublic = 5764801;
  // unsigned long doorPublic = 17807724;

  unsigned long loopSizeCard = getLoopSize(cardPublic);
  unsigned long loopSizeDoor = getLoopSize(doorPublic);

  unsigned long encryptionKey = getEncryptionKey(loopSizeCard, doorPublic);

  std::cout << encryptionKey << std::endl;

  return 0;
}
