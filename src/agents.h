#ifndef AGENTS_H
#define AGENTS_H

#include <functional>
#include <memory>
#include <string>

namespace prototype {

constexpr int kImageSize = 128 * 128 * 3;  // Uses 1 byte to store pixels.

// Signals produced by the world.
struct Signal {
  // Content of the signal.
  int index;
  char signals[kImageSize];

  // Functions for serialization.
  std::string SerializeToString();
  static Signal Deserialize(const std::string& serialized);
};

// The processor processes the signals produced by the world and passes the
// results to the consumers.
class Processor {
 public:
  Processor() = default;
  virtual ~Processor() = default;

  virtual void InPlaceProcess(Signal* signal) = 0;
};

// Makes a default processor used in this prototype.
std::unique_ptr<Processor> MakeDefaultProcessor();

// Consumes the signal.
class Consumer {
 public:
  Consumer(std::function<void(Signal*)> deleter)
      : deleter_(std::move(deleter)) {}
  ~Consumer() = default;

  void Consume(std::unique_ptr<Signal> signal) { deleter_(signal.release()); }

 private:
  std::function<void(Signal*)> deleter_;
};

}  // namespace prototype

#endif  // AGENTS_H
