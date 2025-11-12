CXX = g++
CXXFLAGS = -O2 -Iinclude -Wall
SRC = src/main.cpp src/file_ops.cpp src/main_menu.cpp src/clipboard.cpp \
      src/find_and_replace.cpp src/autosave.cpp src/stats.cpp src/similarity.cpp \
      src/recent_files.cpp src/compression.cpp src/undo.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = Femto-Text-Editor

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)
