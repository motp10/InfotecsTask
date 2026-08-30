#include <ctime>
#include <iomanip>
#include <chrono>

#include "time_utils.h"

std::chrono::system_clock::time_point CurrentTime() {
  return std::chrono::system_clock::now();
}

std::string FormatLocalTime() {
  const auto time = CurrentTime();

  const std::time_t time_t = std::chrono::system_clock::to_time_t(time);

  std::tm local_time{};

  if (localtime_r(&time_t, &local_time) == nullptr) {
    return {};
  }

  std::ostringstream stream;
  stream << std::put_time(
      &local_time,
      "%Y-%m-%d %H:%M:%S");

  return stream.str();
}