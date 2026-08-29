#include <stdexcept>
#include "statistics.h"
#include "../common_lib/time_utils.h"

void Statistics::AddMessage(std::size_t message_length,
                            ImportanceLevel importance,
                            std::chrono::system_clock::time_point received_at) {

    const auto now = CurrentTime();

    if (received_at > now) {
        throw std::invalid_argument(
            "Received time cannot be in the future.");
    }

    message_times_.push_front(received_at);

    auto current = message_times_.begin();

    while (std::next(current) != message_times_.end() &&
        *current < *std::next(current)) {
    std::iter_swap(current, std::next(current));
    ++current;
    }

    const auto one_hour_ago = now - std::chrono::hours(1);

    while (!message_times_.empty() && message_times_.back() < one_hour_ago) {
        message_times_.pop_back();
    }

    min_message_length_ = std::min(min_message_length_, message_length);
    max_message_length_ = std::max(max_message_length_, message_length);

    sum_message_length_ += message_length;

    ++importance_counts_[static_cast<std::size_t>(importance)];
}
std::size_t Statistics::TotalMessages() const {
    size_t result = 0;
    for (const auto count : importance_counts_) {
        result += count;
    }
    return result;
}

std::size_t Statistics::MessagesLastHour() const {
    return message_times_.size();
}

std::size_t Statistics::MinMessageLength() const {
    return min_message_length_;
}

std::size_t Statistics::MaxMessageLength() const {
    return max_message_length_;
}

double Statistics::AverageMessageLength() const {
    const auto total_messages = TotalMessages();
    if (total_messages == 0) {
        return 0.0;
    }
    return static_cast<double>(sum_message_length_) / static_cast<double>(total_messages);
}

std::size_t Statistics::MessagesByImportance(ImportanceLevel importance) const {
    return importance_counts_[static_cast<std::size_t>(importance)];
}