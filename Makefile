###############################################################################
# 
# @file: Makefile
# @author: Sāgararājavarman Lāḍalā
# @date: 2018-06-18
# @brief: This file contains the build instructions for the tetris project
# @version: 1.0
# @license: Unlicense
#
###############################################################################

PROJECT           =  tetris

# Source files
INCLUDES          =  inc
SRC_DIR           =  src
CSRCS             := $(wildcard $(SRC_DIR)/*.cpp)
BUILD_DIR         =  build
RELEASE_DIR       =  release
OBJS              := $(CSRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)


SFML_HOME         = /Users/$(USER)/apps/SFML-3.0.0
SFML_INC          = $(SFML_HOME)/include
SFML_LIB          = $(SFML_HOME)/lib

# Define the target executable
TARGET            = $(BUILD_DIR)/$(RELEASE_DIR)/$(PROJECT)

# Compiler settings
CXX               = g++
CXXSTD            = -std=c++23
CXXFLAGS          = -march=native -O3 -ffast-math -flto \
                    -Wall -Wextra -Wpedantic -Werror \
                    -I${INCLUDES} -I${SFML_INC}
LDFLAGS           = -L${SFML_LIB} -Wl,-rpath,${SFML_LIB}
LDLIBS            = -lsfml-graphics -lsfml-window -lsfml-system

# --- Colors for messages ---
GREEN := \033[0;32m
BLUE  := \033[0;34m
YELLOW:= \033[1;33m
PINK  := \033[1;35m
RESET := \033[0m


default: all

# Default target
all: $(TARGET)

# if building on macOS then you need to trust the SFML libraries
trustsfml:
	@echo "$(YELLOW)[TRUST SFML]$(RESET)"
	@echo "$(PINK)Clearing cache$(RESET)"
	@sudo codesign --force --deep --sign - $(SFML_LIB)/*.dylib
# 	@echo "$(PINK)Removing quarantine attribute$(RESET)"
# 	@xattr -d com.apple.quarantine $(SFML_LIB)/*.dylib
	@echo "$(PINK)Resigning$(RESET)"
	@sudo codesign --force --deep --sign - $(SFML_LIB)/*.dylib

# Build rules
$(TARGET): $(OBJS)
	@echo "$(BLUE)[LINK]$(RESET)    $@"
	@mkdir -p $(dir $@)  # Ensure the directory exists
	@$(CXX) $(LDFLAGS) $(LDLIBS) $^ -o $@

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "$(GREEN)[CXX]$(RESET)     $<"
	@mkdir -p $(dir $@)  # Ensure the directory exists
	@$(CXX) $(CXXSTD) $(CXXFLAGS) -DASSETS_PATH=\"$(CURDIR)/assets\" -c $< -o $@

# Clean up build artifacts
clean:
	@echo "$(YELLOW)[CLEAN]$(RESET)   $@"
	@rm -rf $(BUILD_DIR)

# PHONY commands
.PHONY: all clean
