#pragma once

#include "../common_lib/importance_level.h"

#include <string>

struct Message {
  std::string text;
  ImportanceLevel level;
};
