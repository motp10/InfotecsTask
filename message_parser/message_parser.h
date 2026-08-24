#pragma once

#include "../messages/message.h"

#include <optional>
#include <string>

class MessageParser {
public:
  explicit MessageParser(ImportanceLevel default_level);

  std::optional<Message> ParseMessage(const std::string& text,
                                      const std::string& level);
  const std::string& ErrorMessage() const;

private:
  std::optional<ImportanceLevel> ParseImportanceLevel(
      const std::string& level) const;

  ImportanceLevel default_level_;
  std::string error_message_;
};
