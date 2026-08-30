#include "importance_level.h"

std::optional<const char*> ImportanceLevelToString(ImportanceLevel level) {
  switch (level) {
    case ImportanceLevel::kLow:
      return "LOW";
    case ImportanceLevel::kMedium:
      return "MEDIUM";
    case ImportanceLevel::kHigh:
      return "HIGH";
  }

  return std::nullopt;
}

ImportanceLevel ParseImportanceLevel(const std::string& level_str) {

  if (level_str == "low" || level_str == "LOW") {
    return ImportanceLevel::kLow;
  } else if (level_str == "medium" || level_str == "MEDIUM") {
    return ImportanceLevel::kMedium;
  } else if (level_str == "high" || level_str == "HIGH") {
    return ImportanceLevel::kHigh;
  } else {
    return ImportanceLevel::kUndefiend;
  }
}
