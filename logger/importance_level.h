#pragma once

#include <optional>

enum class ImportanceLevel {
  kLow,
  kMedium,
  kHigh,
};

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