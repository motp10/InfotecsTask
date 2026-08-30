#pragma once
#include "../messages/message_queue.h"
#include "../loggers/i_logger.h"

class Worker {
public:
  Worker(MessageQueue& queue, ILogger& logger)
      : queue_(queue), logger_(logger) {}

  void Run();

private:
  MessageQueue& queue_;
  ILogger& logger_;
};