#include "file_logger.h"
#include "message_formater.h"

FileLogger::FileLogger(const std::string& filename,
                       ImportanceLevel default_importance_level)
  : filename_(filename)
  , default_importance_level_(default_importance_level) {}

FileLoggerResult FileLogger::Open() {
  if (!ImportanceLevelToString(default_importance_level_).has_value()) {
    return FileLoggerResult::kInvalidLevel;
  }

  if (log_file_.is_open()) {
    return FileLoggerResult::kOk;
  }

  log_file_.clear();
  log_file_.open(filename_, std::ios::out | std::ios::app);

  if (!log_file_.is_open()) {
    return FileLoggerResult::kOpenError;
  }

  return FileLoggerResult::kOk;
}

FileLogger::~FileLogger() {
  if (log_file_.is_open()) {
    log_file_.close();
  }
}

LogResult FileLogger::Log(const std::string& message, ImportanceLevel level) {
  if (level < default_importance_level_) {
    return LogResult::kFiltered;
  }

  if (!log_file_.is_open()) {
    return LogResult::kWriteError;
  }

  const FormatResult formatted_message =
      MessageFormatter::FormatMessage(message, level);

  if (!formatted_message.Ok()) {
    if (formatted_message.error == FormatError::kInvalidImportanceLevel) {
      return LogResult::kInvalidLevel;
    }

    return LogResult::kTimestampError;
  }

  log_file_ << formatted_message.text << '\n';


  return log_file_ ? LogResult::kWritten : LogResult::kWriteError;
}

bool FileLogger::SetImportanceLevel(ImportanceLevel new_level) {
  if (!ImportanceLevelToString(new_level).has_value()) {
    return false;
  }

  default_importance_level_ = new_level;
  return true;
}
