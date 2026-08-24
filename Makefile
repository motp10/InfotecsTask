# Пример: make LIB=liblogger.so
#          make SRCS='logger.cpp'

CXX     ?= g++
LIB     ?= liblogger.so
SRCS    ?= $(wildcard *.cpp)
OBJS    := $(SRCS:.cpp=.o)
DEPS    := $(OBJS:.o=.d)

CPPFLAGS ?=
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -Wswitch -Werror -fPIC
LDFLAGS  ?=
LDLIBS   ?=

.PHONY: all clean

all: $(LIB)

$(LIB): $(OBJS)
	@test -n "$(OBJS)" || { echo "Ошибка: не найдены исходные файлы C++"; exit 1; }
	$(CXX) -shared $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	$(RM) $(OBJS) $(DEPS) $(LIB)

-include $(DEPS)
