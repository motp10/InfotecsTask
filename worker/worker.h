#pragma once
#include "../messages/message_queue.h"
#include "../logger/logger.h"

class Worker {
public:
  Worker(MessageQueue& queue, FileLogger& logger)
      : queue_(queue), logger_(logger) {}

  void Run();

private:
  MessageQueue& queue_;
  FileLogger& logger_;
};