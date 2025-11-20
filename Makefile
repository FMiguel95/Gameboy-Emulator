# --- Project Setup ---
TARGET = emulator

# C and C++ sources
C_SRCS   = $(wildcard *.c)
CPP_SRCS = $(wildcard *.cpp) $(wildcard imgui/*.cpp)

# Object files
OBJS = $(C_SRCS:.c=.o) $(CPP_SRCS:.cpp=.o)

# Include paths
INCLUDES = -I. -Iimgui

# Compiler / Linker
CC      = gcc
CXX     = g++
CFLAGS  = -O2 $(INCLUDES) -g
CXXFLAGS = -O2 $(INCLUDES) -g
LDFLAGS = -lSDL3

# Add ImGui dependencies
LDFLAGS += -lstdc++

# --- Build Rules ---

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
