#include "message_parser.h"

#include <algorithm>
#include <cctype>

namespace {

std::string Trim(std::string value) {
  const auto first = std::find_if_not(
      value.begin(), value.end(),
      [](unsigned char character) { return std::isspace(character); });
  const auto last = std::find_if_not(
      value.rbegin(), value.rend(),
      [](unsigned char character) { return std::isspace(character); }).base();

  if (first >= last) {
    return "";
  }

  return std::string(first, last);
}

std::string ToLower(std::string value) {
  std::transform(value.begin(), value.end(), value.begin(),
                 [](unsigned char character) {
                   return static_cast<char>(std::tolower(character));
                 });
  return value;
}

}  // namespace

MessageParser::MessageParser(ImportanceLevel default_level)
    : default_level_(default_level) {}

std::optional<Message> MessageParser::ParseMessage(const std::string& text,
                                                    const std::string& level) {
  error_message_.clear();

  if (Trim(text).empty()) {
    error_message_ = "Message text cannot be empty";
    return std::nullopt;
  }

  const std::string level_text = ToLower(Trim(level));
  if (level_text.empty()) {
    return Message{text, default_level_};
  }

  const auto parsed_level = ParseImportanceLevel(level_text);
  if (!parsed_level.has_value()) {
    error_message_ = "Invalid importance level: " + level;
    return std::nullopt;
  }

  return Message{text, *parsed_level};
}

const std::string& MessageParser::ErrorMessage() const {
  return error_message_;
}

std::optional<ImportanceLevel> MessageParser::ParseImportanceLevel(
    const std::string& level) const {
  if (level == "low") {
    return ImportanceLevel::kLow;
  }
  if (level == "medium") {
    return ImportanceLevel::kMedium;
  }
  if (level == "high") {
    return ImportanceLevel::kHigh;
  }

  return std::nullopt;
}
