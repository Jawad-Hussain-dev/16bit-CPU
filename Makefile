# Makefile for Dungeon Crawler RPG
# Enhanced C++ Edition with Advanced Features

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g
INCLUDES = -Isrc
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/dungeon_crawler

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(OBJDIR) $(BINDIR)

# Main target
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(OBJECTS) -o $@ $(LIBS)
	@echo "Build complete! Run with: ./$(TARGET)"

# Object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Console-only version (without SFML)
console: LIBS =
console: CXXFLAGS += -DCONSOLE_ONLY
console: directories $(TARGET)
	@echo "Console-only version built!"

# Debug build
debug: CXXFLAGS += -DDEBUG -g3 -O0
debug: directories $(TARGET)
	@echo "Debug build complete!"

# Release build
release: CXXFLAGS += -DNDEBUG -O3
release: directories $(TARGET)
	@echo "Release build complete!"

# Clean build files
clean:
	@echo "Cleaning build files..."
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "Clean complete!"

# Install (copy to system directory)
install: $(TARGET)
	@echo "Installing to /usr/local/bin..."
	sudo cp $(TARGET) /usr/local/bin/
	@echo "Installation complete!"

# Uninstall
uninstall:
	@echo "Removing from /usr/local/bin..."
	sudo rm -f /usr/local/bin/dungeon_crawler
	@echo "Uninstallation complete!"

# Run the game
run: $(TARGET)
	./$(TARGET)

# Run with valgrind for memory checking
memcheck: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Format code using clang-format
format:
	@echo "Formatting code..."
	find $(SRCDIR) -name "*.cpp" -o -name "*.h" | xargs clang-format -i
	@echo "Code formatting complete!"

# Static analysis with cppcheck
analyze:
	@echo "Running static analysis..."
	cppcheck --enable=all --std=c++17 $(SRCDIR)/
	@echo "Static analysis complete!"

# Generate documentation with doxygen
docs:
	@echo "Generating documentation..."
	doxygen Doxyfile
	@echo "Documentation generated in docs/html/"

# Package for distribution
package: clean release
	@echo "Creating distribution package..."
	tar -czf dungeon_crawler_v1.0.tar.gz $(SRCDIR)/ Makefile README.md docs/
	@echo "Package created: dungeon_crawler_v1.0.tar.gz"

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build the complete game (default)"
	@echo "  console   - Build console-only version (no SFML required)"
	@echo "  debug     - Build debug version"
	@echo "  release   - Build optimized release version"
	@echo "  clean     - Remove all build files"
	@echo "  install   - Install to system directory"
	@echo "  uninstall - Remove from system directory"
	@echo "  run       - Build and run the game"
	@echo "  memcheck  - Run with valgrind memory checking"
	@echo "  format    - Format code with clang-format"
	@echo "  analyze   - Run static analysis with cppcheck"
	@echo "  docs      - Generate documentation with doxygen"
	@echo "  package   - Create distribution package"
	@echo "  help      - Show this help message"

# Dependencies
$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(SRCDIR)/GameManager.h $(SRCDIR)/GUI.h $(SRCDIR)/Exceptions.h
$(OBJDIR)/GameManager.o: $(SRCDIR)/GameManager.cpp $(SRCDIR)/GameManager.h $(SRCDIR)/Player.h $(SRCDIR)/Room.h $(SRCDIR)/Enemy.h $(SRCDIR)/Leaderboard.h
$(OBJDIR)/Player.o: $(SRCDIR)/Player.cpp $(SRCDIR)/Player.h $(SRCDIR)/Item.h $(SRCDIR)/Container.h $(SRCDIR)/Exceptions.h
$(OBJDIR)/Enemy.o: $(SRCDIR)/Enemy.cpp $(SRCDIR)/Enemy.h $(SRCDIR)/Combatable.h $(SRCDIR)/Renderable.h
$(OBJDIR)/Room.o: $(SRCDIR)/Room.cpp $(SRCDIR)/Room.h $(SRCDIR)/Player.h $(SRCDIR)/Enemy.h $(SRCDIR)/Item.h
$(OBJDIR)/Item.o: $(SRCDIR)/Item.cpp $(SRCDIR)/Item.h $(SRCDIR)/Player.h $(SRCDIR)/Renderable.h $(SRCDIR)/Usable.h
$(OBJDIR)/Leaderboard.o: $(SRCDIR)/Leaderboard.cpp $(SRCDIR)/Leaderboard.h $(SRCDIR)/Algorithms.h
$(OBJDIR)/Algorithms.o: $(SRCDIR)/Algorithms.cpp $(SRCDIR)/Algorithms.h
$(OBJDIR)/GUI.o: $(SRCDIR)/GUI.cpp $(SRCDIR)/GUI.h $(SRCDIR)/GameManager.h $(SRCDIR)/Player.h $(SRCDIR)/Room.h

.PHONY: all directories console debug release clean install uninstall run memcheck format analyze docs package help

