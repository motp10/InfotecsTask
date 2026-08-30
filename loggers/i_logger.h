#pragma once

#include <string>
#include "importance_level.h"

enum class LogResult {
  kWritten,
  kFiltered,
  kInvalidLevel,
  kTimestampError,
  kWriteError,
};

class ILogger {
public:
    virtual ~ILogger() = default;

    virtual LogResult Log(const std::string& message,
                          ImportanceLevel level) = 0;

    virtual bool SetImportanceLevel(ImportanceLevel new_level) = 0;
};