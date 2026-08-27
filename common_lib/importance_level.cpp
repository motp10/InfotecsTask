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