CXX := g++

BUILD_DIR := build
OBJECT_DIR := $(BUILD_DIR)/obj
LIBRARY_DIR := $(BUILD_DIR)/lib
BINARY_DIR := $(BUILD_DIR)/bin

LIBRARY := $(LIBRARY_DIR)/liblogger.so
APPLICATION := $(BINARY_DIR)/logger_app

LIBRARY_SOURCES := logger/logger.cpp
APPLICATION_SOURCES := main.cpp \
                       argument_parser/argument_parser.cpp \
                       message_parser/message_parser.cpp \
                       worker/worker.cpp

LIBRARY_OBJECTS := $(patsubst %.cpp,$(OBJECT_DIR)/%.o,$(LIBRARY_SOURCES))
APPLICATION_OBJECTS := $(patsubst %.cpp,$(OBJECT_DIR)/%.o,$(APPLICATION_SOURCES))
DEPENDENCIES := $(LIBRARY_OBJECTS:.o=.d) $(APPLICATION_OBJECTS:.o=.d)

CPPFLAGS :=
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Wpedantic -Wswitch -Werror -fPIC -pthread
LDFLAGS :=

.PHONY: all library app test clean

all: library app

library: $(LIBRARY)

app: $(APPLICATION)

$(LIBRARY): $(LIBRARY_OBJECTS)
	@mkdir -p $(dir $@)
	$(CXX) -shared $(LDFLAGS) -o $@ $^

$(APPLICATION): $(APPLICATION_OBJECTS) $(LIBRARY)
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) -o $@ $(APPLICATION_OBJECTS) \
		-L$(LIBRARY_DIR) -llogger -pthread \
		-Wl,-rpath,'$$ORIGIN/../lib'

$(OBJECT_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Тесты ещё не добавлены: цель оставлена для последующего подключения.
test:
	@echo "Tests are not implemented yet."

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPENDENCIES)
