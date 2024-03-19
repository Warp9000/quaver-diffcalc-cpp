rwildcard = $(foreach d, $(wildcard $1*), $(call rwildcard, $d/, $2) $(filter $(subst *, %, $2), $d))
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -pedantic -D_GLIBCXX_USE_CXX11_ABI=1 -DYAML_CPP_STATIC_DEFINE=1 -ggdb -g
SOURCES = $(call rwildcard, lib/, *.cpp) $(call rwildcard, src/, *.cpp) $(call rwildcard, yaml-cpp/src/, *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
INCLUDES = -Iyaml-cpp/include -Ilib
EXECUTABLE = QuaverAPI

.PHONY: all clean debug

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)

debug:
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"