# --- Project Setup ---
TARGET = emulator

# Sources
C_SRCS      = $(wildcard *.c)
CPP_SRCS    = $(wildcard *.cpp)
IMGUI_SRCS  = $(wildcard imgui/*.cpp)

# Objects
OBJS       = $(C_SRCS:.c=.o) $(CPP_SRCS:.cpp=.o)
IMGUI_OBJS = $(IMGUI_SRCS:.cpp=.o)
ALL_OBJS   = $(OBJS) $(IMGUI_OBJS)

# Include paths
INCLUDES = -I. -Iimgui

# Compiler / Linker
CC       = gcc
CXX      = g++
CFLAGS   = -O3 $(INCLUDES) -g
CXXFLAGS = -O3 $(INCLUDES) -g
LDFLAGS  = -lSDL3 -Wl,--enable-new-dtags -Wl,-rpath,'$$ORIGIN/lib'

# CFLAGS   = -g -O3 -flto -march=native -mtune=native -fno-plt -fomit-frame-pointer $(INCLUDES)
# CXXFLAGS = -g -O3 -flto -march=native -mtune=native -fno-plt -fomit-frame-pointer $(INCLUDES)
# LDFLAGS  = -lSDL3 -flto -lstdc++ -Wl,--enable-new-dtags -Wl,-rpath,'$$ORIGIN/lib'

# Add ImGui dependencies
LDFLAGS += -lstdc++

# --- Build Rules ---

all: $(TARGET)

$(TARGET): $(ALL_OBJS)
	$(CXX) $(ALL_OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

fclean:
	rm -f $(ALL_OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
