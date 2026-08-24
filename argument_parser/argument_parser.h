#pragma once

#include <string>
#include "importance_level.h"

struct ConsoleOptions {
  std::string log_file;
  ImportanceLevel level;
};

enum class ParseResult {
  kOk,
  kHelpRequested,
  kError,
};

class ArgumentParser {
public:
  ParseResult ParseCommandLineArguments(int argc, char* argv[]);

  const ConsoleOptions& Options() const;
  const std::string& ErrorMessage() const;
  const std::string& ProgramName() const;

  static const char* HelpText();

 private:
  void ParseFileOption(int argc, char* argv[], int index);
  void ParseLevelOption(int argc, char* argv[], int index);

  ConsoleOptions options_;
  std::string error_message_;
  std::string program_name_;
  bool has_file_ = false;
  bool has_level_ = false;
};
