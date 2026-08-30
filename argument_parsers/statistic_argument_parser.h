#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

#include "parse_result.h"

class StatisticArgumentParser {
public:
  struct StatisticOptions {
    std::uint64_t period_seconds = 0;
    std::size_t messages_to_show = 0;
  };

  ParseResult ParseCommandLineArguments(int argc, char* argv[]);

  const StatisticOptions& Options() const;
  const std::string& ErrorMessage() const;

  static const char* HelpText();

private:
  void ParseTimeOption(int argc, char* argv[], int index);
  void ParseNumberOption(int argc, char* argv[], int index);

  StatisticOptions options_;
  std::string error_message_;
  bool has_time_ = false;
  bool has_number_ = false;
};
