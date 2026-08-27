#pragma once

#include <optional>

enum class ImportanceLevel {
  kLow,
  kMedium,
  kHigh,
};

std::optional<const char*> ImportanceLevelToString(ImportanceLevel level);