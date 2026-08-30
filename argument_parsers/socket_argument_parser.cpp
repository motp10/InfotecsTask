#include "socket_argument_parser.h"

#include <charconv>
#include <cstdint>
#include <limits>
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

struct PortParseResult {
  uint16_t port = 0;
  bool ok = false;
};

PortParseResult ParsePort(const std::string& port_str) {
  unsigned int port = 0;
  const auto [end, error] = std::from_chars(
      port_str.data(), port_str.data() + port_str.size(), port);

  if (error != std::errc{} || end != port_str.data() + port_str.size() ||
      port == 0 || port > std::numeric_limits<uint16_t>::max()) {
    return {};
  }

  return {static_cast<uint16_t>(port), true};
}

}  // namespace

ParseResult SocketArgumentParser::ParseCommandLineArguments(int argc, char* argv[]) {
  options_ = ConsoleOptions{};
  error_message_.clear();
  program_name_ = argc > 0 ? argv[0] : "logger_app";
  has_level_ = false;
  has_host_ = false;
  has_port_ = false;

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

  const PortParseResult parsed_port = ParsePort(argv[index + 1]);
  if (!parsed_port.ok) {
    error_message_ = "Invalid port: " + std::string(argv[index + 1]);
    return;
  }

  options_.port = parsed_port.port;
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
