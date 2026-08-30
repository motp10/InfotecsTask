#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "test_cases.h"

int main(int argc, char* argv[]) {
  struct TestCase {
    const char* name;
    void (*run)();
  };

  const std::vector<TestCase> tests{
      {"TestArgumentParsers", TestArgumentParsers},
      {"TestFileLogger", TestFileLogger},
      {"TestMessageDeserializer", TestMessageDeserializer},
      {"TestMessageParser", TestMessageParser},
      {"TestMessageQueue", TestMessageQueue},
      {"TestStatistics", TestStatistics},
  };

  bool has_failures = false;

  for (const auto& test : tests) {
    if (argc > 1 && std::string(argv[1]) != test.name) {
      continue;
    }

    try {
      test.run();
      std::cout << "[PASS] " << test.name << '\n';
    } catch (const std::exception& error) {
      std::cerr << "[FAIL] " << test.name << ": " << error.what() << '\n';
      has_failures = true;
    }
  }

  return has_failures ? 1 : 0;
}
