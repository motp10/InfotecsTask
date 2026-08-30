#include <string>
#include "../common_lib/importance_level.h"
#include "../messages/message.h"

enum class DeserializeError {
  kNone,
  kInvalidFormat,
  kUnknownImportanceLevel,
  kMissingText,
};

struct DeserializeResult {
  Message message{};
  DeserializeError error = DeserializeError::kNone;

  bool Ok() const {
    return error == DeserializeError::kNone;
  }
};

class MessageDeserializer {
public:
    MessageDeserializer() = default;
    ~MessageDeserializer() = default;
    static DeserializeResult Deserialize(const std::string& message);
 };
