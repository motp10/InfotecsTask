#pragma once

#include "../lib/importance_level.h"

#include <string>

struct Message {
  std::string text;
  ImportanceLevel level;
};
