#include <chrono>
#include <cmath>

#include "statistics/statistics.h"
#include "test_framework.h"

// Проверяет агрегаты, распределение по уровням и удаление сообщений старше часа.
void TestStatistics() {
  Statistics statistics;
  CHECK(statistics.TotalMessages() == 0);
  CHECK(statistics.MessagesLastHour() == 0);
  CHECK(statistics.AverageMessageLength() == 0.0);

  const auto now = std::chrono::system_clock::now();
  statistics.AddMessage(10, ImportanceLevel::kLow, now);
  statistics.AddMessage(30, ImportanceLevel::kHigh, now);

  CHECK(statistics.TotalMessages() == 2);
  CHECK(statistics.MessagesLastHour() == 2);
  CHECK(statistics.MinMessageLength() == 10);
  CHECK(statistics.MaxMessageLength() == 30);
  CHECK(std::abs(statistics.AverageMessageLength() - 20.0) < 0.001);
  CHECK(statistics.MessagesByImportance(ImportanceLevel::kLow) == 1);
  CHECK(statistics.MessagesByImportance(ImportanceLevel::kMedium) == 0);
  CHECK(statistics.MessagesByImportance(ImportanceLevel::kHigh) == 1);

  Statistics expiring_statistics;
  expiring_statistics.AddMessage(5, ImportanceLevel::kLow,
                                 now - std::chrono::hours(2));
  expiring_statistics.AddMessage(6, ImportanceLevel::kMedium, now);
  CHECK(expiring_statistics.MessagesLastHour() == 1);

  bool threw = false;
  try {
    statistics.AddMessage(1, ImportanceLevel::kLow,
                          now + std::chrono::hours(1));
  } catch (const std::invalid_argument&) {
    threw = true;
  }
  CHECK(threw);
}
