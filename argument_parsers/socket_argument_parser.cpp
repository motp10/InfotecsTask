#include "socket_argument_parser.h"

#include <cstdint>
#include <stdexcept>
#include <string_view>

namespace {

    constexpr std::string_view kHostOptionShort = "-h";
    constexpr std::string_view kHostOptionLong = "--host";

    constexpr std::string_view kPortOptionShort = "-p";
    constexpr std::string_view kPortOptionLong = "--port";

    constexpr std::string_view kLevelOptionShort = "-l";
    constexpr std::string_view kLevelOptionLong = "--level";

    constexpr std::string_view kHelpOptionShort = "-?";
    constexpr std::string_view kHelpOptionLong = "--help";

    ImportanceLevel ParseImportanceLevel(
        const std::string& level_str) {

    if (level_str == "low") {
        return ImportanceLevel::kLow;
    } else if (level_str == "medium") {
        return ImportanceLevel::kMedium;
    } else if (level_str == "high") {
        return ImportanceLevel::kHigh;
    }

    throw std::invalid_argument(
        "Invalid importance level: " + level_str);
}

uint16_t ParsePort(const std::string& port_str) {
  try {
    const unsigned long port = std::stoul(port_str);

    if (port == 0 || port > UINT16_MAX) {
      throw std::invalid_argument(
          "Port must be between 1 and 65535");
    }

    return static_cast<uint16_t>(port);

  } catch (...) {
    throw std::invalid_argument(
        "Invalid port: " + port_str);
  }
}

}  // namespace

ParseResult SocketArgumentParser::ParseCommandLineArguments(int argc, char* argv[]) {
  options_ = ConsoleOptions{};
  error_message_.clear();
  program_name_ = argc > 0 ? argv[0] : "logger_app";
  bool has_level_ = false;
  bool has_host_ = false;
  bool has_port_ = false; 

  for (int index = 1; index < argc; ++index) {
    const std::string_view argument(argv[index]);

    if (argument == kHelpOptionShort || argument == kHelpOptionLong) {
      return ParseResult::kHelpRequested;
    }

    const bool is_host_option =
        argument == kHostOptionShort || argument == kHostOptionLong;
    const bool is_port_option =
        argument == kPortOptionShort || argument == kPortOptionLong;
    const bool is_level_option =
        argument == kLevelOptionShort || argument == kLevelOptionLong;

    if (is_host_option) {
      ParseHostOption(argc, argv, index);
      if (!error_message_.empty()) {
        return ParseResult::kError;
      }

      ++index;
      continue;
    }

    if (is_port_option) {
      ParsePortOption(argc, argv, index);
      if (!error_message_.empty()) {
        return ParseResult::kError;
      }

      ++index;
      continue;
    }

    if (is_level_option) {
      ParseLevelOption(argc, argv, index);
      if (!error_message_.empty()) {
        return ParseResult::kError;
      }

      ++index;
      continue;
    }

    error_message_ = "Unexpected argument: " + std::string(argument);
    return ParseResult::kError;
  }

  if (!has_host_) {
    error_message_ = "The: --host option is required";
    return ParseResult::kError;
  }

  if (!has_port_) {
    error_message_ = "The: --port option is required";
    return ParseResult::kError;
  }

  if (!has_level_) {
    error_message_ = "The: --level option is required";
    return ParseResult::kError;
  }

  return ParseResult::kOk;
}

void SocketArgumentParser::ParseHostOption(int argc, char* argv[], int index) {

  if (has_host_) {
    error_message_ =
        "The " + std::string(argv[index]) +
        " option is specified multiple times";
    return;
  }

  if (index + 1 >= argc) {
    error_message_ =
        "The " + std::string(argv[index]) +
        " option requires an argument";
    return;
  }

  options_.host = argv[index + 1];

  if (options_.host.empty()) {
    error_message_ =
        "The " + std::string(argv[index]) +
        " option requires a non-empty argument";
    return;
  }

  has_host_ = true;
}

void SocketArgumentParser::ParsePortOption(int argc, char* argv[], int index) {

  if (has_port_) {
    error_message_ =
        "The " + std::string(argv[index]) +
        " option is specified multiple times";
    return;
  }

  if (index + 1 >= argc) {
    error_message_ =
        "The " + std::string(argv[index]) +
        " option requires an argument";
    return;
  }

  try {
    options_.port = ParsePort(argv[index + 1]);
  } catch (const std::invalid_argument& error) {
    error_message_ = error.what();
    return;
  }

  has_port_ = true;
}

void SocketArgumentParser::ParseLevelOption(int argc, char* argv[], int index) {
  if (has_level_) {
    error_message_ = "The " + std::string(argv[index]) +
                     " option is specified multiple times";
    return;
  }

  if (index + 1 >= argc) {
    error_message_ = "The " + std::string(argv[index]) +
                     " option requires an argument";
    return;
  }

  options_.level = ParseImportanceLevel(argv[index + 1]);
  has_level_ = true;
}

const SocketArgumentParser::ConsoleOptions& SocketArgumentParser::Options() const {
  return options_;
}

const std::string& SocketArgumentParser::ErrorMessage() const {
  return error_message_;
}

const std::string& SocketArgumentParser::ProgramName() const {
  return program_name_;
}

const char* SocketArgumentParser::HelpText() {
  return
      "Usage: socket_logger "
      "--host <host> --port <port> "
      "--level <low|medium|high>\n"
      "\n"
      "Options:\n"
      "  -h, --host <host>     "
      "Server host.\n"
      "  -p, --port <port>     "
      "Server port.\n"
      "  -l, --level <level>   "
      "Minimum level: low, medium, or high.\n"
      "  -?, --help            "
      "Show this help message.\n";
}