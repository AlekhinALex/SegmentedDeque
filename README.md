# SegmentedDeque: A Qt-based Data Structure Visualization Tool

SegmentedDeque is a C++ application that provides an interactive graphical interface for managing and manipulating sequence data structures. It offers a robust implementation of a segmented deque data structure with support for multiple data types and comprehensive sequence operations.

The application combines the efficiency of custom data structures with the user-friendly interface capabilities of Qt. It supports various data types including integers, doubles, strings, complex numbers, and custom person objects. Users can perform operations like appending, prepending, inserting, sorting, filtering, and searching on sequences while visualizing the results in real-time through a modern Qt-based GUI.

## Repository Structure
```
.
├── CMakeLists.txt              # Main build configuration file
├── controllers/                # Application control logic
│   ├── MainController.cpp     # Core controller implementation
│   └── MainController.hpp     # Controller interface definitions
├── models/                    # Data structure implementations
│   ├── inc/                  # Header files for data structures
│   │   ├── arraySequence.hpp
│   │   ├── dynamicArray.hpp
│   │   ├── linkedList.hpp
│   │   ├── listSequence.hpp
│   │   ├── segmentedDeque.hpp
│   │   └── sequence.hpp
│   └── types/                # Custom data types
│       ├── complex.hpp       # Complex number implementation
│       └── person.hpp        # Person data type implementation
└── views/                    # UI components
    ├── createSequenceDialog.cpp
    ├── createSequenceDialog.h
    ├── mainwindow.cpp
    ├── mainwindow.h
    ├── operationdialog.cpp
    └── operationdialog.h
```

## Usage Instructions
### Prerequisites
- C++17 compatible compiler
- CMake 3.16 or higher
- Qt6 or Qt5 with Widgets component
- Build tools (Make/Ninja)

### Installation

#### MacOS
```bash
# Install prerequisites
brew install qt cmake ninja

# Build the project
mkdir build && cd build
cmake -G Ninja ..
ninja

# Run the application
./SegmentedDeque
```

#### Linux
```bash
# Install prerequisites (Ubuntu/Debian)
sudo apt-get install qt6-base-dev cmake build-essential

# Build the project
mkdir build && cd build
cmake ..
make

# Run the application
./SegmentedDeque
```

#### Windows
```batch
# Install prerequisites (using vcpkg)
vcpkg install qt:x64-windows

# Build the project
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=[path_to_vcpkg]/scripts/buildsystems/vcpkg.cmake ..
cmake --build .

# Run the application
.\Debug\SegmentedDeque.exe
```

### Quick Start
1. Launch the application
2. Click "Add" to create a new sequence
3. Select the data type (int, double, string, person, or complex)
4. Use the operation buttons to manipulate the sequence:
   - Append/Prepend: Add elements at the beginning or end
   - Insert: Add elements at specific positions
   - Sort: Order the sequence
   - Filter: Apply conditions to filter elements
   - Search: Find subsequences

### More Detailed Examples
```cpp
// Creating an integer sequence
createSequence("myInts", ValueType::Int, 32);

// Adding elements
appendToSequence("myInts", 42);
prependToSequence("myInts", 10);
insertAtSequence("myInts", 1, 25);

// Sorting
sortSequence("myInts", true); // ascending order

// Filtering
where("myInts", "newInts", "x > 20");
```

## Data Flow
The application follows a Model-View-Controller pattern where sequence operations flow from the UI through the controller to the underlying data structures.

```ascii
[UI Layer (Views)] <-> [Controller Layer] <-> [Model Layer (Sequences)]
     |                       |                      |
User Input             Data Validation        Data Storage
     |                       |                      |
Display Updates    Operation Handling    Structure Management
```

Key component interactions:
1. User initiates operations through the main window
2. MainController validates inputs and manages sequence operations
3. SegmentedDeque implements the core data structure operations
4. Model updates trigger UI refresh through Qt's signal/slot mechanism
5. Error handling propagates from model through controller to UI