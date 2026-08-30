#pragma once

#include <stdexcept>
#include <string>

class TestFailure : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

inline void Check(bool condition, const char* expression, const char* test_name) {
  if (!condition) {
    throw TestFailure(std::string(test_name) + ": failed: " + expression);
  }
}

#define CHECK(expression) Check((expression), #expression, __func__)
