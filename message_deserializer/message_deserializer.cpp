#include "message_deserializer.h"

DeserializeResult MessageDeserializer::Deserialize(const std::string& data) {

  const std::size_t level_start = data.find("] [");

  if (level_start == std::string::npos) {
    return {{}, DeserializeError::kInvalidFormat};
  }

  const std::size_t level_begin = level_start + 3;
  const std::size_t level_end = data.find(']', level_begin);

  if (level_end == std::string::npos) {
    return {{}, DeserializeError::kInvalidFormat};
  }

  const std::string level_string = data.substr(level_begin, level_end - level_begin);

  const ImportanceLevel level = ParseImportanceLevel(level_string);

  if (level == ImportanceLevel::kUndefiend) {
    return {{}, DeserializeError::kUnknownImportanceLevel};
  }

  const std::size_t text_begin = level_end + 2;

  if (text_begin > data.size()) {
    return {{}, DeserializeError::kMissingText};
  }

  const std::string text = data.substr(text_begin);

  return {{text, level}, DeserializeError::kNone};
}
