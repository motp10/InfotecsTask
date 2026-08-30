#include <filesystem>
#include <fstream>
#include <string>

#include "loggers/file_logger.h"
#include "loggers/message_formater.h"
#include "test_framework.h"

// Проверяет запись, фильтрацию и передачу ошибки форматирования без исключения.
void TestFileLogger() {
  const std::filesystem::path log_path =
      std::filesystem::temp_directory_path() / "infotex_file_logger_test.log";
  std::filesystem::remove(log_path);

  {
    FileLogger logger(log_path.string(), ImportanceLevel::kMedium);
    CHECK(logger.Log("ignored", ImportanceLevel::kLow) == LogResult::kFiltered);
    CHECK(logger.Log("saved", ImportanceLevel::kHigh) == LogResult::kWritten);
    CHECK(logger.SetImportanceLevel(ImportanceLevel::kHigh));
    CHECK(logger.Log("also ignored", ImportanceLevel::kMedium) ==
          LogResult::kFiltered);
    CHECK(logger.Log("invalid", static_cast<ImportanceLevel>(99)) ==
          LogResult::kInvalidLevel);
  }

  const FormatResult invalid_format = MessageFormatter::FormatMessage(
      "invalid", static_cast<ImportanceLevel>(99));
  CHECK(!invalid_format.Ok());
  CHECK(invalid_format.error == FormatError::kInvalidImportanceLevel);

  std::ifstream log_file(log_path);
  std::string line;
  std::getline(log_file, line);
  CHECK(line.find("[HIGH] saved") != std::string::npos);
  CHECK(!std::getline(log_file, line));

  std::filesystem::remove(log_path);
}
