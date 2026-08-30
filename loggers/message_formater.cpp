#include "message_formater.h"
#include "../common_lib/time_utils.h"

FormatResult MessageFormatter::FormatMessage(const std::string& message,
                                             ImportanceLevel level) {
    auto time = FormatLocalTime();

    if (time.empty()) {
        return {{}, FormatError::kTimestampError};
    }

    auto level_str = ImportanceLevelToString(level);

    if (!level_str.has_value()) {
        return {{}, FormatError::kInvalidImportanceLevel};
    }

    return {
        "[" + time + "] [" + level_str.value() + "] " + message,
        FormatError::kNone
    };
}
