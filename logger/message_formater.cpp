#include <ctime>
#include <iomanip>
#include <optional>
#include <sstream>
#include <stdexcept>

#include "message_formater.h"

namespace {

std::string CurrentLocalTime() {
    const std::time_t now = std::time(nullptr);
    std::tm local_time{};

    localtime_r(&now, &local_time);
    if (localtime_r(&now, &local_time) == nullptr) {
        return {};
    }
    
    std::ostringstream stream;
    stream << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");

    return stream.str();
}

}  // namespace

std::string MessageFormatter::FormatMessage(const std::string& message,
                                            ImportanceLevel level) {
    std::time_t now = std::time(nullptr);
    char timestamp[20];
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    return "[" + std::string(timestamp) + "] [" +
           std::to_string(static_cast<int>(level)) + "] " + message;
}