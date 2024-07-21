CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
TARGET = run_catan
SOURCES = main.cpp catangame.cpp player.cpp board.cpp card.cpp tile.cpp catan.cpp
HEADERS = catan.hpp

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

catan: $(TARGET)
	./$(TARGET) catan

.PHONY: clean catan

clean:
	rm -f $(TARGET)