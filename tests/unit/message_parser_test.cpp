#include "message_parser/message_parser.h"
#include "test_framework.h"

// Проверяет уровни важности, валидацию текста и сброс ошибки после успешного разбора.
void TestMessageParser() {
  MessageParser parser(ImportanceLevel::kMedium);

  const auto default_level = parser.ParseMessage("message", "  ");
  CHECK(default_level.has_value());
  CHECK(default_level->text == "message");
  CHECK(default_level->level == ImportanceLevel::kMedium);

  const auto explicit_level = parser.ParseMessage("message", " HIGH ");
  CHECK(explicit_level.has_value());
  CHECK(explicit_level->level == ImportanceLevel::kHigh);

  CHECK(!parser.ParseMessage(" \t", "low").has_value());
  CHECK(parser.ErrorMessage() == "Message text cannot be empty");
  CHECK(!parser.ParseMessage("message", "urgent").has_value());
  CHECK(parser.ErrorMessage() == "Invalid importance level: urgent");

  const auto after_error = parser.ParseMessage("new message", "low");
  CHECK(after_error.has_value());
  CHECK(after_error->level == ImportanceLevel::kLow);
  CHECK(parser.ErrorMessage().empty());
}
