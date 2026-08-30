#include <deque>
#include <chrono>
#pragma once

#include <array>

#include "importance_level.h"

class Statistics {
public:
    void AddMessage(std::size_t message_length,
                    ImportanceLevel importance,
                    std::chrono::system_clock::time_point received_at);

    std::size_t TotalMessages() const;
    std::size_t MessagesLastHour() const;
    std::size_t MinMessageLength() const;
    std::size_t MaxMessageLength() const;
    double AverageMessageLength() const;
    std::size_t MessagesByImportance(ImportanceLevel importance) const;

private:
  std::deque<std::chrono::system_clock::time_point> message_times_;
  std::size_t max_message_length_ = 0;
  std::size_t min_message_length_ = UINT64_MAX;
  std::size_t sum_message_length_ = 0;
  std::array<std::size_t, 3> importance_counts_{};
};
