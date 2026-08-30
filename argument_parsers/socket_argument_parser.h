#pragma once

#include <string>
#include <cstdint>

#include "importance_level.h"
#include "parse_result.h"

class SocketArgumentParser {
private:
    struct ConsoleOptions {
        std::string host;
        uint16_t port;
        ImportanceLevel level;
    };

public:
  ParseResult ParseCommandLineArguments(int argc, char* argv[]);

  const ConsoleOptions& Options() const;
  const std::string& ErrorMessage() const;
  const std::string& ProgramName() const;

  static const char* HelpText();

 private:

  void ParseLevelOption(int argc, char* argv[], int index);
  void ParseHostOption(int argc, char* argv[], int index);
  void ParsePortOption(int argc, char* argv[], int index);

  ConsoleOptions options_;
  std::string error_message_;
  std::string program_name_;
  bool has_level_ = false;
  bool has_host_ = false;
  bool has_port_ = false; 
};
