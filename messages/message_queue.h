#pragma once

#include "message.h"

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include <utility>

class MessageQueue {
public:
  bool Push(Message message);

  std::optional<Message> WaitAndPop();

  void Close();

private:
  std::queue<Message> messages_;
  std::mutex mutex_;
  std::condition_variable condition_variable_;
  bool is_closed_ = false;
};
