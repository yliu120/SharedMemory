#include "agents.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

namespace prototype {

std::string Signal::SerializeToString() {
  std::string serialized;
  serialized.reserve(sizeof(Signal));
  serialized.append(reinterpret_cast<const char*>(&index), sizeof(index));
  serialized.append(signals, kImageSize);
  return serialized;
}

Signal Signal::Deserialize(const std::string& serialized) {
  if (serialized.size() != sizeof(Signal)) {
    // Crashes loudly. Or it is better to return a absl::StatusOr<>.
    std::cerr << "Incorrect serialized string for signals.\n";
    std::abort();
  }

  Signal signal;
  std::memcpy(&signal.index, serialized.data(), sizeof(signal.index));
  std::memcpy(signal.signals, serialized.data() + sizeof(signal.index),
              kImageSize);
  return signal;
}

class DefaultProcessor : public Processor {
 public:
  void InPlaceProcess(Signal* signal) override {
    // Let's simply implement a in-place reverse of the array in the signal.
    int start = 0;
    int end = kImageSize - 1;

    char* arr = signal->signals;
    while (start < end) {
      std::swap(arr[start], arr[end]);
      start++;
      end--;
    }
  }
};

std::unique_ptr<Processor> MakeDefaultProcessor() {
  return std::make_unique<DefaultProcessor>();
}

}  // namespace prototype