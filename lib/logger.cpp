#include "logger.h"

#include <ctime>
#include <iomanip>
#include <optional>
#include <sstream>
#include <stdexcept>

namespace {

std::optional<const char*> ImportanceLevelToString(ImportanceLevel level) {
  switch (level) {
    case ImportanceLevel::kLow:
      return "LOW";
    case ImportanceLevel::kMedium:
      return "MEDIUM";
    case ImportanceLevel::kHigh:
      return "HIGH";
  }

  return std::nullopt;
}

std::string CurrentLocalTime() {
    const std::time_t now = std::time(nullptr);
    std::tm local_time{};

    localtime_r(&now, &local_time);

    std::ostringstream stream;
    stream << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");

    return stream.str();
}

}  // namespace

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
  std::lock_guard<std::mutex> lock(mutex_);

  const auto level_name = ImportanceLevelToString(level);
  if (!level_name.has_value()) {
    return LogResult::kInvalidLevel;
  }

  if (level < default_importance_level_) {
    return LogResult::kFiltered;
  }

  if (!log_file_.is_open()) {
    return LogResult::kWriteError;
  }

  const auto timestamp = CurrentLocalTime();
  if (timestamp.empty()) {
    return LogResult::kTimestampError;
  }

  log_file_ << '[' << timestamp << "] [" << *level_name << "] " << message << '\n';
  
  return log_file_ ? LogResult::kWritten : LogResult::kWriteError;
}

bool FileLogger::SetImportanceLevel(ImportanceLevel new_level) {
  std::lock_guard<std::mutex> lock(mutex_);

  if (!ImportanceLevelToString(new_level).has_value()) {
    return false;
  }

  default_importance_level_ = new_level;
  return true;
}
