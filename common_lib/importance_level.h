#pragma once

#include <optional>
#include <string>

enum class ImportanceLevel {
  kLow,
  kMedium,
  kHigh,
  kUndefiend
};

std::optional<const char*> ImportanceLevelToString(ImportanceLevel level);

ImportanceLevel ParseImportanceLevel(const std::string& level_str);