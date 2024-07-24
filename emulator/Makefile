CXX := clang++
CXXFLAGS := -O0 -std=c++17
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
OUT_DIR := bin
TARGET := emu

ALL: setup $(OUT_DIR)/$(TARGET)

setup:
	@mkdir -p $(OUT_DIR)

$(OUT_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OUT_DIR)/$(TARGET)
