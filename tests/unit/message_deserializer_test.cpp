#include "message_deserializer/message_deserializer.h"
#include "test_framework.h"

// Проверяет извлечение текста и уровня, а также отклонение некорректных сообщений.
void TestMessageDeserializer() {
  const DeserializeResult result = MessageDeserializer::Deserialize(
      "[2026-08-30 12:00:00] [HIGH] payload");
  CHECK(result.Ok());
  CHECK(result.message.text == "payload");
  CHECK(result.message.level == ImportanceLevel::kHigh);

  const DeserializeResult medium_result = MessageDeserializer::Deserialize(
      "[2026-08-30 12:00:00] [MEDIUM] another payload");
  CHECK(medium_result.Ok());
  CHECK(medium_result.message.level == ImportanceLevel::kMedium);

  const DeserializeResult invalid_format =
      MessageDeserializer::Deserialize("bad input");
  CHECK(!invalid_format.Ok());
  CHECK(invalid_format.error == DeserializeError::kInvalidFormat);

  const DeserializeResult invalid_level = MessageDeserializer::Deserialize(
      "[2026-08-30] [UNKNOWN] payload");
  CHECK(!invalid_level.Ok());
  CHECK(invalid_level.error == DeserializeError::kUnknownImportanceLevel);
}
