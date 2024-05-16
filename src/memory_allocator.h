#include <string>

namespace prototype {

class MemoryAllocator {
 public:
  explicit MemoryAllocator(char* pool) : pool_(pool) {}

  char* Allocate();
  void Free();

 private:
  char* pool_ = nullptr;
};

}  // namespace prototype