#pragma once

#include <string>
#include "importance_level.h"

enum class FormatError {
    kNone,
    kTimestampError,
    kInvalidImportanceLevel,
};

struct FormatResult {
    std::string text;
    FormatError error = FormatError::kNone;

    bool Ok() const {
        return error == FormatError::kNone;
    }
};

class MessageFormatter {
public:
    static FormatResult FormatMessage(const std::string& message,
                                      ImportanceLevel level);
};
