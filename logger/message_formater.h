#pragma once

#include <string>
#include "importance_level.h"

#include <stdexcept>

class InvalidImportanceLevelError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class TimestampError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class MessageFormatter {
public:
    static std::string FormatMessage(const std::string& message,
                                     ImportanceLevel level);        

};