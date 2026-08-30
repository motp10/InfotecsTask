#pragma once

#include <fstream>
#include <string>
#include "importance_level.h"
#include "i_logger.h"

enum class FileLoggerResult {
  kOk,
  kInvalidLevel,
  kOpenError,
};

class FileLogger final : public ILogger {
public:
  FileLogger(const std::string& filename,
              ImportanceLevel default_importance_level);
  ~FileLogger();

  FileLogger(const FileLogger&) = delete;
  FileLogger& operator=(const FileLogger&) = delete;

  FileLoggerResult Open();
  LogResult Log(const std::string& message, ImportanceLevel level) override;

  bool SetImportanceLevel(ImportanceLevel new_level) override;

private:
  std::string filename_;
  std::ofstream log_file_;
  ImportanceLevel default_importance_level_;
};
