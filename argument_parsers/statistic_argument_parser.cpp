#include "statistic_argument_parser.h"

#include <charconv>
#include <limits>
#include <string_view>

namespace {

constexpr std::string_view kTimeOptionShort = "-t";
constexpr std::string_view kTimeOptionLong = "--time";
constexpr std::string_view kNumberOptionShort = "-n";
constexpr std::string_view kNumberOptionLong = "--number";
constexpr std::string_view kHelpOptionShort = "-h";
constexpr std::string_view kHelpOptionLong = "--help";

bool ParsePositiveNumber(std::string_view text, std::uint64_t* value) {
  std::uint64_t parsed_value = 0;
  const auto [end, error] = std::from_chars(
      text.data(), text.data() + text.size(), parsed_value);

  if (error != std::errc{} || end != text.data() + text.size() ||
      parsed_value == 0) {
    return false;
  }

  *value = parsed_value;
  return true;
}

}  // namespace

ParseResult StatisticArgumentParser::ParseCommandLineArguments(
    int argc, char* argv[]) {
  options_ = StatisticOptions{};
  error_message_.clear();
  has_time_ = false;
  has_number_ = false;

  for (int index = 1; index < argc; ++index) {
    const std::string_view argument(argv[index]);

    if (argument == kHelpOptionShort || argument == kHelpOptionLong) {
      return ParseResult::kHelpRequested;
    }

    if (argument == kTimeOptionShort || argument == kTimeOptionLong) {
      ParseTimeOption(argc, argv, index);
    } else if (argument == kNumberOptionShort || argument == kNumberOptionLong) {
      ParseNumberOption(argc, argv, index);
    } else {
      error_message_ = "Unexpected argument: " + std::string(argument);
      return ParseResult::kError;
    }

    if (!error_message_.empty()) {
      return ParseResult::kError;
    }

    ++index;
  }

  if (!has_time_) {
    error_message_ = "The: --time option is required";
    return ParseResult::kError;
  }

  if (!has_number_) {
    error_message_ = "The: --number option is required";
    return ParseResult::kError;
  }

  return ParseResult::kOk;
}

void StatisticArgumentParser::ParseTimeOption(int argc, char* argv[], int index) {
  if (has_time_) {
    error_message_ = "The " + std::string(argv[index]) +
                     " option is specified multiple times";
    return;
  }

  if (index + 1 >= argc) {
    error_message_ = "The " + std::string(argv[index]) +
                     " option requires an argument";
    return;
  }

  std::uint64_t period_seconds = 0;
  if (!ParsePositiveNumber(argv[index + 1], &period_seconds)) {
    error_message_ = "Invalid time period: " + std::string(argv[index + 1]);
    return;
  }

  options_.period_seconds = period_seconds;
  has_time_ = true;
}

void StatisticArgumentParser::ParseNumberOption(
    int argc, char* argv[], int index) {
  if (has_number_) {
    error_message_ = "The " + std::string(argv[index]) +
                     " option is specified multiple times";
    return;
  }

  if (index + 1 >= argc) {
    error_message_ = "The " + std::string(argv[index]) +
                     " option requires an argument";
    return;
  }

  std::uint64_t messages_to_show = 0;
  if (!ParsePositiveNumber(argv[index + 1], &messages_to_show) ||
      messages_to_show > std::numeric_limits<std::size_t>::max()) {
    error_message_ = "Invalid message count: " + std::string(argv[index + 1]);
    return;
  }

  options_.messages_to_show = static_cast<std::size_t>(messages_to_show);
  has_number_ = true;
}

const StatisticArgumentParser::StatisticOptions&
StatisticArgumentParser::Options() const {
  return options_;
}

const std::string& StatisticArgumentParser::ErrorMessage() const {
  return error_message_;
}

const char* StatisticArgumentParser::HelpText() {
  return "Usage: statistic_app --time <seconds> --number <count>\n"
         "\n"
         "Options:\n"
         "  -t, --time <seconds>  Print statistics at this interval.\n"
         "  -n, --number <count>  Print statistics after this many messages.\n"
         "  -h, --help            Show this help message.\n";
}
