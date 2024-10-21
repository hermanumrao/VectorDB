# Compiler
CXX := g++

# Compiler flags
CXXFLAGS := -Wall -Wextra
LDFLAGS := -larrow -lparquet

# Optimization flag (can be overridden from command line)
OPTFLAG := -O2

# Directories
SRC_DIR := src
INCLUDE_DIR := src/include
BUILD_DIR := build

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(INCLUDE_DIR)/*.cpp)
OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

# Main target
TARGET := $(BUILD_DIR)/main

# Phony targets
.PHONY: all clean debug release

# Default target
all: $(TARGET)

# Debug target
debug: OPTFLAG := -g
debug: all

# Release target
release: OPTFLAG := -O6
release: all

# Linking
$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(OPTFLAG) $^ -o $@ $(LDFLAGS)

# Compiling
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(OPTFLAG) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR)

# Print variables (for debugging)
print-%:
	@echo $* = $($*)
