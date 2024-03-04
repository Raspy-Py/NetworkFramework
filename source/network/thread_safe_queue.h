#ifndef CLIENT_THREADSAFEQUEUE_H
#define CLIENT_THREADSAFEQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <exception>

template<typename T>
class ThreadSafeQueue {
 public:
  ThreadSafeQueue() = default;
  ThreadSafeQueue(const ThreadSafeQueue& other) = delete;
  ThreadSafeQueue& operator=(const ThreadSafeQueue& other) = delete;

  void Push(T value) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(std::move(value));
    cv_.notify_one();
  }

  bool TryPop(T& value) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (queue_.empty()) {
      return false;
    }
    value = std::move(queue_.front());
    queue_.pop();
    return true;
  }

  void WaitAndPop(T& value) {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this]{ return !queue_.empty(); });
    value = std::move(queue_.front());
    queue_.pop();
  }

  bool Empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
  }

  size_t Size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
  }

 private:
  mutable std::mutex mutex_;
  std::queue<T> queue_;
  std::condition_variable cv_;
};


#endif //CLIENT_THREADSAFEQUEUE_H