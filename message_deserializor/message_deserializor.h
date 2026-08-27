#include <string>
#include "../common_lib/importance_level.h"
#include "../messages/message.h"

class MessageDeserializer {
public:
    MessageDeserializer() = default;
    ~MessageDeserializer() = default;
    static Message Deserialize(const std::string& message);
 };