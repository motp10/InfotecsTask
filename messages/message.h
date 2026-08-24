#pragma once

#include "../logger/importance_level.h"

#include <string>

struct Message {
  std::string text;
  ImportanceLevel level;
};
