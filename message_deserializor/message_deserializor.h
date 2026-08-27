#include <string>

class MessageDeserializor {
public:
    MessageDeserializor() = default;
    ~MessageDeserializor() = default;
    static std::string Deserialize(const std::string& message);
 };