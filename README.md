# Enhanced Dungeon Crawler

A sophisticated C++ dungeon crawler game demonstrating advanced object-oriented programming concepts, data structures, algorithms, and modern C++ features with SFML graphics.

## 🎮 Features

### Core Gameplay
- **Multiple Player Classes**: Warrior, Mage, and Rogue with unique abilities
- **Diverse Enemy Types**: Goblins, Witches, Bosses, and Dragons with AI behaviors
- **Turn-based Combat System**: Priority queue-based combat with strategic depth
- **Inventory Management**: Template-based inventory system with item categories
- **Pathfinding AI**: BFS, DFS, and A* algorithms for enemy movement
- **Procedural Generation**: Recursive maze generation algorithms

### Technical Highlights
- **Advanced OOP**: Inheritance, polymorphism, abstract classes, operator overloading
- **Modern C++**: Templates, lambdas, smart pointers, exception handling
- **Data Structures**: Priority queues, stacks, queues, hash maps, graphs
- **Algorithms**: Pathfinding, sorting, recursive generation, combat calculations
- **SFML Graphics**: Hardware-accelerated 2D graphics with animation system
- **Event-driven Input**: Flexible input binding system with context management

## 🛠️ Technical Requirements Met

### Core OOP Concepts ✅
- **Classes & Objects**: All game entities implemented as proper classes
- **Encapsulation**: Private members with public interfaces
- **Constructors/Destructors**: Proper resource management
- **Operator Overloading**: Position arithmetic, entity comparison
- **Inheritance & Polymorphism**: Player/Enemy hierarchies with virtual functions
- **Abstract Classes**: Pure virtual interfaces (Entity, Drawable, Updatable)

### Data Structures & Algorithms ✅
- **Dynamic Arrays/Linked Lists**: STL containers for game collections
- **Stack/Queue**: Path history, combat actions, enemy queues
- **Priority Queues**: Turn-based combat system
- **Recursion**: Maze generation, pathfinding algorithms
- **Sorting Algorithms**: Template-based merge sort implementation
- **Graph Algorithms**: A*, BFS, DFS pathfinding

### Advanced C++ Features ✅
- **Templates**: Generic inventory system, sorting algorithms
- **Exception Handling**: Custom game exceptions with proper error recovery
- **STL Containers**: Vector, map, unordered_map, queue, stack
- **Lambdas**: Event callbacks, input handling, AI behaviors
- **Smart Pointers**: Automatic memory management

## 🚀 Building the Project

### Prerequisites
- **C++17 compatible compiler** (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake 3.16+**
- **SFML 2.5+**

### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential cmake libsfml-dev
```

### Windows (vcpkg)
```bash
vcpkg install sfml
```

### macOS (Homebrew)
```bash
brew install cmake sfml
```

### Build Instructions
```bash
# Clone the repository
git clone <repository-url>
cd enhanced-dungeon-crawler

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build . --config Release

# Run the game
./EnhancedDungeonCrawler
```

## 🎯 Controls

### Movement
- **WASD** or **Arrow Keys**: Move player
- **Space**: Attack
- **E**: Interact with objects

### Interface
- **I**: Open/Close inventory
- **ESC**: Pause game or return to menu
- **Enter**: Start game from main menu
- **1-9**: Use inventory items (in inventory screen)

### Combat
- **Space**: Attack enemy
- **Q**: Use special ability
- **Mouse**: Target selection (planned)

## 🏗️ Architecture Overview

### Core Systems
```
Game (Main Controller)
├── Renderer (SFML Graphics)
├── InputManager (Event Handling)
├── CombatSystem (Turn-based Combat)
├── PathFinder (AI Navigation)
└── Entity Management
```

### Class Hierarchy
```
Entity (Abstract Base)
├── GameEntity (Drawable + Updatable)
    ├── Player (Abstract)
    │   ├── Warrior
    │   ├── Mage
    │   └── Rogue
    └── Enemy (Abstract)
        ├── Goblin
        ├── Witch
        ├── Boss
        └── Dragon

Item (Abstract Base)
├── Weapon
├── Potion
├── Treasure
└── Key
```

### Template Systems
- **Inventory<T>**: Generic container with sorting and searching
- **PriorityContainer<T>**: Template priority queue wrapper
- **Sorting Algorithms**: Generic merge sort implementation

## 🎨 Asset Requirements

The game expects the following assets in the `assets/` directory:

### Textures (`assets/textures/`)
- `warrior.png` - Warrior player sprite
- `mage.png` - Mage player sprite  
- `rogue.png` - Rogue player sprite
- `goblin.png` - Goblin enemy sprite
- `witch.png` - Witch enemy sprite
- `boss.png` - Boss enemy sprite
- `wall.png` - Wall tile texture
- `floor.png` - Floor tile texture

### Fonts (`assets/fonts/`)
- `arial.ttf` - Main UI font

*Note: The game will run without assets but may display placeholder graphics.*

## 🧪 Design Patterns Used

- **Strategy Pattern**: Different player classes and enemy AI behaviors
- **Observer Pattern**: Event system for game state changes
- **Factory Pattern**: Entity creation and spawning
- **State Pattern**: Game state management (Menu, Playing, Combat, etc.)
- **Template Method**: Base entity update/render cycles
- **Singleton Pattern**: Resource managers and game systems

## 📊 Performance Features

- **Object Pooling**: Efficient entity management
- **Spatial Partitioning**: Optimized collision detection
- **Level-of-Detail**: Conditional rendering based on distance
- **Memory Management**: Smart pointers and RAII principles
- **Cache-Friendly**: Data structure optimization for performance

## 🔧 Configuration

### Input Bindings
Input bindings can be customized by editing `config/input.cfg`:
```
move_up=W
move_down=S
move_left=A
move_right=D
attack=Space
inventory=I
```

### Game Settings
Game settings are stored in `config/settings.cfg`:
```
resolution_width=1024
resolution_height=768
fullscreen=false
vsync=true
sound_volume=0.8
```

## 🐛 Debugging

### Debug Build
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

### Common Issues
1. **SFML not found**: Ensure SFML is properly installed and in PATH
2. **Missing assets**: Game will run but may show placeholder graphics
3. **Performance issues**: Try reducing window size or disabling VSync

## 📈 Future Enhancements

- **Multiplayer Support**: Network-based cooperative gameplay
- **Level Editor**: Visual dungeon creation tools
- **Mod Support**: Lua scripting for custom content
- **Advanced Graphics**: Particle systems and lighting effects
- **Audio System**: Sound effects and background music
- **Save System**: Game state persistence
- **Achievement System**: Progress tracking and rewards

## 📚 Learning Objectives Demonstrated

This project showcases mastery of:

1. **Object-Oriented Design**: Proper class hierarchies and relationships
2. **Memory Management**: Smart pointers and resource handling
3. **Algorithm Implementation**: Pathfinding, sorting, and game algorithms
4. **Template Programming**: Generic and reusable code components
5. **Exception Safety**: Robust error handling and recovery
6. **Modern C++ Features**: C++17 features and best practices
7. **Graphics Programming**: 2D rendering and animation systems
8. **Game Architecture**: Scalable and maintainable game systems

## 📄 License

This project is created for educational purposes as part of a university assignment demonstrating advanced C++ programming concepts.

## 👥 Contributing

This is an academic project, but suggestions and improvements are welcome for educational purposes.

---

**Note**: This enhanced version significantly expands upon the original console-based dungeon crawler, incorporating advanced programming concepts while maintaining clean, educational code structure.

