CXX ?= g++
CXXFLAGS ?= -std=c++20 -Wall -Wextra -Wpedantic

BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
BIN := $(BUILD_DIR)/blink

PKG_CONFIG := pkg-config
GTK_CFLAGS := $(shell $(PKG_CONFIG) --cflags gtk+-3.0 2>/dev/null)
GTK_LIBS := $(shell $(PKG_CONFIG) --libs gtk+-3.0 2>/dev/null)
VTE_CFLAGS := $(shell $(PKG_CONFIG) --cflags vte-2.91)
VTE_LIBS := $(shell $(PKG_CONFIG) --libs vte-2.91)

CPPFLAGS += -Iinclude -I/usr/include/vte-2.91 -Iinclude/vte $(GTK_CFLAGS) $(VTE_CFLAGS)
LDLIBS += $(GTK_LIBS) $(VTE_LIBS)

SRCS := \
	src/main.cpp \
	src/app/app.cpp \
	src/clipboard/clipboard.cpp \
	src/clipboard/history.cpp \
	src/search/regex.cpp \
	src/search/search.cpp \
	src/terminal/pty.cpp \
	src/terminal/input.cpp \
	src/terminal/renderer.cpp \
	src/terminal/scrollback.cpp \
	src/terminal/session.cpp \
	src/terminal/shell.cpp \
	src/terminal/terminal.cpp \
	src/ui/dialogs.cpp \
	src/ui/settings.cpp \
	src/ui/tabs.cpp \
	src/ui/titlebar.cpp \
	src/ui/window.cpp \
	src/utils/filesystem.cpp \
	src/utils/logger.cpp \
	src/utils/process.cpp \
	src/utils/string.cpp \
	src/utils/time.cpp

OBJS := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

all: $(BIN)

$(BIN): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(OBJS) -o $@ $(LDLIBS)

$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
