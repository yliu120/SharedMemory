#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace prototype {

// A trivial implementation of threadpool.
class ThreadPool {
 public:
  explicit ThreadPool(int num_workers);

  void EnqueueWork(std::function<void()> task);

  // Waits until all works are done.
  void Quiesce();

 private:
  std::vector<std::thread> workers_;
  std::queue<std::function<void()>> queue_;
  std::mutex mu_;
  std::condition_variable cond_var_;
  bool stop_ = false;
};

}  // namespace prototype

#endif  // THREADPOOL_H