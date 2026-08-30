#include <atomic>
#include <chrono>
#include <thread>

#include "messages/message_queue.h"
#include "test_framework.h"

// Проверяет FIFO-порядок, обработку закрытия очереди и разблокировку ожидающего потока.
void TestMessageQueue() {
  MessageQueue queue;
  CHECK(queue.Push({"first", ImportanceLevel::kLow}));
  CHECK(queue.Push({"second", ImportanceLevel::kHigh}));

  const auto first_message = queue.WaitAndPop();
  CHECK(first_message.has_value());
  CHECK(first_message->text == "first");

  queue.Close();

  const auto message = queue.WaitAndPop();
  CHECK(message.has_value());
  CHECK(message->text == "second");
  CHECK(!queue.WaitAndPop().has_value());
  CHECK(!queue.Push({"late", ImportanceLevel::kHigh}));

  MessageQueue waiting_queue;
  std::atomic<bool> was_unblocked = false;
  std::thread waiting_thread([&] {
    was_unblocked = !waiting_queue.WaitAndPop().has_value();
  });
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  waiting_queue.Close();
  waiting_thread.join();
  CHECK(was_unblocked.load());
}
