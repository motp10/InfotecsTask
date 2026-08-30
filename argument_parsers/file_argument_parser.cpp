#include "file_argument_parser.h"

#include <string_view>
#include <stdexcept>

namespace {
  constexpr std::string_view kFileOptionShort = "-f";
  constexpr std::string_view kFileOptionLong = "--file";
  constexpr std::string_view kLevelOptionShort = "-l";
  constexpr std::string_view kLevelOptionLong = "--level";
  constexpr std::string_view kHelpOptionShort = "-h";
  constexpr std::string_view kHelpOptionLong = "--help";
}  // namespace

ParseResult FileArgumentParser::ParseCommandLineArguments(int argc, char* argv[]) {
  options_ = ConsoleOptions{};
  error_message_.clear();
  program_name_ = argc > 0 ? argv[0] : "logger_app";
  has_file_ = false;
  has_level_ = false;

  for (int index = 1; index < argc; ++index) {
    const std::string_view argument(argv[index]);

    if (argument == kHelpOptionShort || argument == kHelpOptionLong) {
      return ParseResult::kHelpRequested;
    }

    const bool is_file_option =
        argument == kFileOptionShort || argument == kFileOptionLong;
    const bool is_level_option =
        argument == kLevelOptionShort || argument == kLevelOptionLong;

    if (is_file_option) {
      ParseFileOption(argc, argv, index);
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

  if (!has_file_) {
    error_message_ = "The: --file option is required";
    return ParseResult::kError;
  }

  if (!has_level_) {
    error_message_ = "The: --level option is required";
    return ParseResult::kError;
  }

  return ParseResult::kOk;
}

void FileArgumentParser::ParseFileOption(int argc, char* argv[], int index) {
  if (has_file_) {
    error_message_ = "The " + std::string(argv[index]) +
                     " option is specified multiple times";
    return;
  }

  if (index + 1 >= argc) {
    error_message_ = "The " + std::string(argv[index]) +
                     " option requires an argument";
    return;
  }

  options_.log_file = argv[index + 1];
  has_file_ = true;
}

void FileArgumentParser::ParseLevelOption(int argc, char* argv[], int index) {
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

  if (options_.level == ImportanceLevel::kUndefiend) {
    error_message_ = "Invalid importance level: " + std::string(argv[index + 1]);
    return;
  }
  
  has_level_ = true;
}

const FileArgumentParser::ConsoleOptions& FileArgumentParser::Options() const {
  return options_;
}

const std::string& FileArgumentParser::ErrorMessage() const {
  return error_message_;
}

const std::string& FileArgumentParser::ProgramName() const {
  return program_name_;
}

const char* FileArgumentParser::HelpText() {
  return "Usage: logger_app --file <path> --level <low|medium|high>\n"
         "\n"
         "Options:\n"
         "  -f, --file <path>     Log file path.\n"
         "  -l, --level <level>   Minimum level: low, medium, or high.\n"
         "  -h, --help            Show this help message.\n";
}
