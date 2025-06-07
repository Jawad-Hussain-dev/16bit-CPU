# Dungeon Crawler RPG - Enhanced C++ Edition

A comprehensive dungeon crawler game demonstrating advanced C++ programming concepts, object-oriented design, and modern software development practices.

## 🎮 Game Overview

Navigate through a dangerous dungeon filled with enemies, treasures, and traps. Use strategy, combat skills, and resource management to reach the exit and achieve the highest score!

### Features
- **Multiple Enemy Types**: Goblins, Orcs, Dragons, and Witches with unique abilities
- **Rich Item System**: Weapons, potions, and treasures with different properties
- **Dynamic Room Types**: Normal rooms, trap rooms, treasure rooms, boss rooms, and shops
- **Character Progression**: Level up, gain experience, and improve your abilities
- **Leaderboard System**: Track high scores and compete with other players
- **Dual Interface**: Both console and GUI modes available

## 🔧 Technical Requirements Met

This project demonstrates mastery of the following C++ concepts:

### Core OOP Concepts ✅
- **Classes & Objects**: All game entities implemented as proper classes
- **Encapsulation**: Private members with public interfaces
- **Constructors/Destructors**: Proper resource management
- **Operator Overloading**: Comparison and stream operators for game entities
- **Inheritance & Polymorphism**: Enemy and Item hierarchies with virtual functions
- **Abstract Classes**: Pure virtual interfaces (Renderable, Combatable, Usable)

### Data Structures & Algorithms ✅
- **Dynamic Arrays/Linked Lists**: Custom Container template class
- **Stack/Queue**: Room navigation and combat management
- **Recursion**: Pathfinding algorithms (BFS/DFS)
- **Sorting Algorithms**: Merge sort and quick sort for leaderboards
- **STL Containers**: vector, map, queue, stack throughout the codebase

### Advanced C++ Features ✅
- **Templates**: Generic Container class and sorting algorithms
- **Exception Handling**: Custom exception classes with proper error recovery
- **STL Containers**: Extensive use of standard library containers
- **Lambdas**: Event system and functional programming patterns
- **Smart Pointers**: Modern memory management with shared_ptr and unique_ptr

## 🏗️ Architecture

### Class Hierarchy

```
Renderable (Abstract)
├── Item
│   ├── Weapon (also implements Usable)
│   ├── Potion (also implements Usable)
│   └── Treasure
├── Enemy
│   ├── Goblin
│   ├── Orc
│   ├── Dragon
│   └── Witch
├── Player (also implements Combatable)
└── Room
    ├── TrapRoom
    ├── TreasureRoom
    ├── BossRoom
    └── ShopRoom

Combatable (Abstract)
├── Player
└── Enemy (and all derived classes)

Usable (Abstract)
├── Weapon
└── Potion
```

### Key Components

- **GameManager**: Central game controller managing game state and flow
- **Container<T>**: Template class for type-safe collections
- **Leaderboard**: Score tracking with sorting algorithms
- **GUI**: Simple SFML-based graphical interface
- **Exception System**: Custom exception hierarchy for error handling

## 🚀 Building and Running

### Prerequisites

- C++17 compatible compiler (GCC 7+ or Clang 5+)
- SFML 2.5+ (for GUI mode, optional)
- Make utility

### Quick Start

```bash
# Clone the repository
git clone <repository-url>
cd dungeon-crawler-rpg

# Build the game
make

# Run the game
make run
```

### Build Options

```bash
# Console-only version (no SFML required)
make console

# Debug build with debugging symbols
make debug

# Optimized release build
make release

# Clean build files
make clean
```

### Installation

```bash
# Install to system directory
sudo make install

# Uninstall
sudo make uninstall
```

## 🎯 How to Play

### Console Mode
1. Enter your hero's name
2. Navigate through rooms using the menu system
3. Fight enemies, collect items, and manage resources
4. Reach the exit to win!

### GUI Mode
1. Use mouse clicks for actions
2. Visual representation of game state
3. Interactive buttons for all game actions
4. Real-time health and status bars

### Game Mechanics

- **Health**: Manage your health carefully - death ends the game
- **Moves**: Limited action points - use them wisely
- **Combat**: Choose between light attacks (guaranteed) or heavy attacks (risky but powerful)
- **Items**: Weapons increase damage, potions restore health
- **Gold**: Earn gold from defeating enemies, spend it in shops
- **Experience**: Level up to increase health and moves

## 📊 Scoring System

Your final score is calculated based on:
- Player level (100 points per level)
- Gold collected (1 point per gold)
- Enemies defeated (50 points each)
- Rooms explored (25 points each)
- Completion bonus (1000 points)
- Efficiency bonus (10 points per move remaining)

## 🛠️ Development Tools

### Code Quality
```bash
# Format code
make format

# Static analysis
make analyze

# Memory checking
make memcheck
```

### Documentation
```bash
# Generate documentation
make docs
```

### Packaging
```bash
# Create distribution package
make package
```

## 📁 Project Structure

```
dungeon-crawler-rpg/
├── src/                    # Source code
│   ├── main.cpp           # Main application entry point
│   ├── GameManager.*      # Game controller
│   ├── Player.*           # Player class
│   ├── Enemy.*            # Enemy hierarchy
│   ├── Item.*             # Item hierarchy
│   ├── Room.*             # Room hierarchy
│   ├── Container.h        # Template container class
│   ├── Algorithms.*       # Sorting and pathfinding
│   ├── Leaderboard.*      # Score tracking
│   ├── GUI.*              # Graphical interface
│   ├── Exceptions.h       # Custom exception classes
│   ├── Renderable.h       # Abstract rendering interface
│   ├── Combatable.h       # Abstract combat interface
│   └── Usable.h           # Abstract usable item interface
├── docs/                  # Documentation
├── Makefile              # Build system
└── README.md             # This file
```

## 🎓 Educational Value

This project serves as a comprehensive example of:

1. **Object-Oriented Design**: Proper use of inheritance, polymorphism, and encapsulation
2. **Modern C++**: Templates, smart pointers, STL containers, and exception handling
3. **Software Architecture**: Clean separation of concerns and modular design
4. **Game Development**: Core game mechanics and user interface design
5. **Build Systems**: Professional Makefile with multiple targets
6. **Documentation**: Comprehensive README and code documentation

## 🐛 Troubleshooting

### Common Issues

1. **SFML not found**: Install SFML development libraries or use console mode
2. **Compilation errors**: Ensure C++17 support and proper includes
3. **Runtime crashes**: Check for proper exception handling and memory management

### Debug Mode
```bash
make debug
gdb ./bin/dungeon_crawler
```

## 🤝 Contributing

This project is designed for educational purposes. Feel free to:
- Add new enemy types or items
- Implement additional room types
- Enhance the GUI interface
- Add new algorithms or data structures
- Improve game balance and mechanics

## 📜 License

This project is created for educational purposes as part of a university assignment demonstrating advanced C++ programming concepts.

## 🏆 Assignment Requirements Checklist

### Core OOP Concepts ✅
- [x] Classes & Objects
- [x] Encapsulation with getters/setters
- [x] Constructors/Destructors (including copy constructors)
- [x] Operator Overloading
- [x] Inheritance & Polymorphism
- [x] Abstract Classes with pure virtual functions

### Data Structures & Algorithms ✅
- [x] Dynamic Arrays/Linked Lists (Container template)
- [x] Stack/Queue (room navigation, combat queue)
- [x] Recursion (pathfinding algorithms)
- [x] Sorting Algorithms (merge sort, quick sort)
- [x] STL Containers (vector, map, queue, stack)

### Advanced C++ Features ✅
- [x] Templates (Container class, sorting algorithms)
- [x] Exception Handling (custom exception hierarchy)
- [x] STL Containers (extensive usage)
- [x] Lambdas (event system)
- [x] Smart Pointers (shared_ptr, unique_ptr)

### Additional Requirements ✅
- [x] Proper file structure (.h/.cpp files)
- [x] Build system (Makefile)
- [x] Documentation (README)
- [x] Demonstration executable
- [x] Design report (this README serves as the report)

---

**Total Lines of Code**: ~2000+  
**Classes Implemented**: 15+  
**Design Patterns Used**: Factory, Observer, Template Method  
**C++ Standard**: C++17  

This project demonstrates comprehensive understanding of advanced C++ programming concepts while creating an engaging and functional game experience.

