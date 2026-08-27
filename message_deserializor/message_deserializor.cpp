#include "message_deserializor.h"

Message MessageDeserializer::Deserialize(const std::string& data) const {

  const std::size_t level_start = data.find("] [");

  if (level_start == std::string::npos) {
    throw std::invalid_argument("Invalid message format");
  }

  const std::size_t level_begin = level_start + 3;
  const std::size_t level_end = data.find(']', level_begin);

  if (level_end == std::string::npos) {
    throw std::invalid_argument("Invalid message format");
  }

  const std::string level_string = data.substr(level_begin, level_end - level_begin);

  ImportanceLevel level;

  if (level_string == "LOW") {
    level = ImportanceLevel::kLow;
  } else if (level_string == "MEDIUM") {
    level = ImportanceLevel::kMedium;
  } else if (level_string == "HIGH") {
    level = ImportanceLevel::kHigh;
  } else {
    throw std::invalid_argument(
        "Unknown importance level: " + level_string);
  }

  const std::size_t text_begin = level_end + 2;

  if (text_begin > data.size()) {
    throw std::invalid_argument("Message text is missing");
  }

  const std::string text = data.substr(text_begin);

  return Message(text, level);
}