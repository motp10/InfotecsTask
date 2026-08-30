#include "file_logger.h"
#include "message_formater.h"
#include <ctime>
#include <iomanip>
#include <optional>
#include <sstream>
#include <stdexcept>

FileLogger::FileLogger(const std::string& filename,
                       ImportanceLevel default_importance_level)
  : default_importance_level_(default_importance_level) {
  if (!ImportanceLevelToString(default_importance_level_).has_value()) {
    throw std::invalid_argument("Invalid default importance level");
  }

  log_file_.open(filename, std::ios::out | std::ios::app);

  if (!log_file_.is_open()) {
    throw std::runtime_error("Unable to open log file: " + filename);
  }
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
