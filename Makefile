CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2 -Iinclude
TARGET = gallery_eval_plus
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) src/*.o

run: $(TARGET)
	./$(TARGET)
