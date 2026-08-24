#pragma once

#include <fstream>
#include <mutex>
#include <string>

enum class ImportanceLevel {
  kLow,
  kMedium,
  kHigh,
};

enum class LogResult {
  kWritten,
  kFiltered,
  kInvalidLevel,
  kTimestampError,
  kWriteError,
};

class FileLogger {
 public:
  FileLogger(const std::string& filename,
             ImportanceLevel default_importance_level);
  ~FileLogger();

  FileLogger(const FileLogger&) = delete;
  FileLogger& operator=(const FileLogger&) = delete;

  LogResult Log(const std::string& message, ImportanceLevel level);

  // Returns true if the importance level was successfully set, false otherwise.
  bool SetImportanceLevel(ImportanceLevel new_level);

 private:
  std::ofstream log_file_;
  ImportanceLevel default_importance_level_;
  std::mutex mutex_;
};