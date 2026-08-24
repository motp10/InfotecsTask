#include "worker.h"

void Worker::Run() {
    while (const auto message = queue_.WaitAndPop()) {
      logger_.Log(message->text, message->level);
    }
}