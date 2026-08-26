#pragma once

#include <string>
#include "importance_level.h"

class MessageFormatter {
public:
    static std::string FormatMessage(const std::string& message,
                                        ImportanceLevel level);        

};