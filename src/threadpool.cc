#include "threadpool.h"

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace prototype {

ThreadPool::ThreadPool(int num_workers) {
  workers_.reserve(num_workers);
  for (int i = 0; i < num_workers; ++i) {
    workers_.emplace_back([this] {
      while (true) {
        std::function<void()> task;
        {
          std::unique_lock<std::mutex> lock(mu_);
          cond_var_.wait(lock, [this] { return stop_ || !queue_.empty(); });
          if (stop_ && queue_.empty()) {
            return;
          }
          task = std::move(queue_.front());
          queue_.pop();
        }
        task();
      }
    });
  }
}

void ThreadPool::EnqueueWork(std::function<void()> task) {
  {
    std::unique_lock<std::mutex> lock(mu_);
    queue_.push(std::move(task));
  }
  cond_var_.notify_one();
}

void ThreadPool::Quiesce() {
  {
    std::unique_lock<std::mutex> lock(mu_);
    stop_ = true;
  }
  cond_var_.notify_all();
  for (auto& worker : workers_) {
    worker.join();
  }
}

}  // namespace prototype