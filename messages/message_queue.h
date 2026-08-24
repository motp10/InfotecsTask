#pragma once

#include "message.h"

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include <utility>

class MessageQueue {
 public:
  bool Push(Message message) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (is_closed_) {
      return false;
    }

    messages_.push(std::move(message));
    condition_variable_.notify_one();
    return true;
  }

  std::optional<Message> WaitAndPop() {
    std::unique_lock<std::mutex> lock(mutex_);
    condition_variable_.wait(lock,
                             [this] { return is_closed_ || !messages_.empty(); });

    if (messages_.empty()) {
      return std::nullopt;
    }

    Message message = std::move(messages_.front());
    messages_.pop();
    return message;
  }

  void Close() {
    std::lock_guard<std::mutex> lock(mutex_);
    is_closed_ = true;
    condition_variable_.notify_all();
  }

 private:
  std::queue<Message> messages_;
  std::mutex mutex_;
  std::condition_variable condition_variable_;
  bool is_closed_ = false;
};
