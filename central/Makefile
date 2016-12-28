CXX := g++
TARGET := central_controller

# list up objects
SRC_DIRS := src
SRCS := $(shell find $(SRC_DIRS) -name *.cpp)
OBJS := $(SRCS:.cpp=.o)

# list up includes
INCLUDE_DIRS := $(shell find $(SRC_DIRS) -type d)
INCLUDES := $(addprefix -I,$(INCLUDE_DIRS))
INCLUDES := $(INCLUDES) -I../thirdparty/eigen -I../thirdparty/boost/boost

CXXFLAGS := -Wall -O2 -pipe -std=c++14 $(INCLUDES)

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)
