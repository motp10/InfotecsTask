#include <ctime>
#include <iomanip>
#include <optional>
#include <sstream>
#include <stdexcept>

#include "message_formater.h"
#include "../common_lib/time_utils.h"


std::string MessageFormatter::FormatMessage(const std::string& message,
                                            ImportanceLevel level) {
    auto time = CurrentLocalTime();

    if (time.empty()) {
        throw TimestampError("Failed to get current local time");
    }

    auto level_str = ImportanceLevelToString(level);

    if (!level_str.has_value()) {
        throw InvalidImportanceLevelError("Invalid importance level");
    }

    return "[" + time + "] [" +
           level_str.value() + "] " + message;
}