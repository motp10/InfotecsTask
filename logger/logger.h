#pragma once

#include <fstream>
#include <mutex>
#include <string>
#include "importance_level.h"

enum class LogResult {
  kWritten,
  kFiltered,
  kInvalidLevel,
  kTimestampError,
  kWriteError,
};

class ILogger {
public:
    virtual ~ILogger() = default;

    virtual LogResult Log(const std::string& message,
                          ImportanceLevel level) = 0;

    virtual bool SetImportanceLevel(ImportanceLevel new_level) = 0;
};

class FileLogger final : public ILogger {
public:
  FileLogger(const std::string& filename,
              ImportanceLevel default_importance_level);
  ~FileLogger();

  FileLogger(const FileLogger&) = delete;
  FileLogger& operator=(const FileLogger&) = delete;

  LogResult Log(const std::string& message, ImportanceLevel level) override;

  // Returns true if the importance level was successfully set, false otherwise.
  bool SetImportanceLevel(ImportanceLevel new_level) override;

private:
  std::ofstream log_file_;
  ImportanceLevel default_importance_level_;
  std::mutex mutex_;
};

class SocketLogger final : public ILogger {
public:
    SocketLogger(const std::string& host,
                 uint16_t port,
                 ImportanceLevel default_importance_level);

    LogResult Log(const std::string& message,
                  ImportanceLevel level) override;

    bool SetImportanceLevel(ImportanceLevel new_level) override;

private:
    int socket_;
    ImportanceLevel default_importance_level_;
    std::mutex mutex_;
};