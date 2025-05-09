# C++ Sequence Data Structures Library with Segmented Deque Implementation

A modern C++ template library providing efficient sequence data structures with a focus on the SegmentedDeque implementation. This library offers flexible and performant sequence operations through a combination of array-based and linked list-based storage strategies.

The library implements a collection of sequence data structures with a common interface, featuring a novel SegmentedDeque that combines the benefits of both array and linked list implementations. The SegmentedDeque provides efficient operations at both ends of the sequence while maintaining good memory locality through fixed-size array segments. This makes it particularly well-suited for applications requiring frequent insertions and deletions at either end of the sequence while maintaining good cache performance.

## Repository Structure
```
.
├── CMakeLists.txt          # CMake build configuration (C++14)
└── inc/                    # Header files directory
    ├── arraySequence.hpp   # Array-based sequence implementation
    ├── dynamicArray.hpp    # Dynamic array container
    ├── linkedList.hpp      # Linked list implementation
    ├── listSequence.hpp    # List-based sequence implementation
    ├── segmentedDeque.hpp  # Main segmented deque implementation
    └── sequence.hpp        # Base sequence interface
```

## Usage Instructions
### Prerequisites
- CMake (version 3.10 or higher)
- C++14 compatible compiler
- Google Test framework
- pthread library (for Linux systems)

### Installation

#### MacOS
```bash
# Install prerequisites
brew install cmake
xcode-select --install

# Clone and build
git clone <repository-url>
cd <repository-name>
mkdir build && cd build
cmake ..
make
```

#### Linux
```bash
# Install prerequisites
sudo apt-get update
sudo apt-get install cmake g++ libgtest-dev

# Clone and build
git clone <repository-url>
cd <repository-name>
mkdir build && cd build
cmake ..
make
```

#### Windows
```powershell
# Install prerequisites (using vcpkg)
vcpkg install gtest:x64-windows

# Clone and build
git clone <repository-url>
cd <repository-name>
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

### Quick Start
```cpp
#include "segmentedDeque.hpp"

// Create a segmented deque with default segment size (32)
SegmentedDeque<int> deque;

// Add elements
deque.append(1);
deque.prepend(0);
deque.insertAt(2, 2);

// Access elements
int first = deque.getFirst();  // 0
int last = deque.getLast();    // 2
int middle = deque.get(1);     // 1
```

### More Detailed Examples
```cpp
// Create a segmented deque with custom segment size
SegmentedDeque<double> deque(64);

// Perform sequence operations
deque.append(1.0);
deque.append(2.0);

// Create an immutable copy with modifications
auto newDeque = deque.appendImmutable(3.0);

// Get a subsequence
auto subseq = deque.getSubsequence(0, 1);

// Concatenate sequences
auto other = new SegmentedDeque<double>();
other->append(4.0);
deque.concat(other);
```

### Troubleshooting

#### Common Issues

1. Compilation Errors
```bash
# Enable verbose CMake output
cmake -DCMAKE_VERBOSE_MAKEFILE=ON ..
```

2. Memory Issues
- Use valgrind for memory leak detection:
```bash
valgrind --leak-check=full ./tests
```

3. Performance Issues
- Monitor segment size impact:
```cpp
// Adjust segment size based on your use case
SegmentedDeque<T> deque(optimal_segment_size);
```

## Data Flow
The SegmentedDeque implements a hybrid approach combining array and linked list characteristics. Data is stored in fixed-size array segments managed by a linked list, providing efficient access and modification at both ends.

```ascii
[List Node] -> [Array Segment 1] -> [Array Segment 2] -> ... -> [Array Segment N]
     |               |                    |                           |
     v               v                    v                           v
  Metadata     [Data Block 1]      [Data Block 2]             [Data Block N]
```

Key Component Interactions:
- Segments are managed through a ListSequence of ArraySequence objects
- Each segment maintains a fixed size for optimal memory usage
- Element access requires segment location calculation and offset computation
- Insertions may trigger segment reallocation or creation
- Deletions may lead to segment merging or removal
- Operations maintain O(1) amortized time complexity for end operations