# C++ Sequence Data Structures Library with Dynamic Arrays and Linked Lists

A modern C++ template library providing efficient and flexible sequence data structures with support for dynamic arrays, linked lists, and segmented deques. The library offers both mutable and immutable operations, iterator support, and comprehensive error handling.

This library implements several sequence data structures with a common interface, allowing for easy interchangeability while maintaining type safety and performance. It features dynamic arrays for constant-time random access, linked lists for efficient insertions and deletions, and segmented deques that combine the benefits of both approaches. The implementation uses C++14 features and follows modern C++ best practices.

## Repository Structure
```
.
├── CMakeLists.txt          # CMake build configuration
├── inc/                    # Header files directory
│   ├── arraySequence.hpp   # Array-based sequence implementation
│   ├── dynamicArray.hpp    # Dynamic array container
│   ├── linkedList.hpp      # Linked list implementation
│   ├── listSequence.hpp    # List-based sequence implementation
│   ├── segmentedDeque.hpp  # Hybrid sequence implementation
│   └── sequence.hpp        # Base sequence interface
├── tests/                  # Test files directory
│   ├── arraySequenceTests.cpp
│   ├── dynamicArrayTests.cpp
│   ├── functionPointerTest.cpp
│   ├── linkedListTests.cpp
│   ├── listSequenceTests.cpp
│   └── segmentedDequeTest.cpp
└── types/                  # Custom type definitions
    ├── complex.hpp         # Complex number type
    └── person.hpp          # Person data type
```

## Usage Instructions
### Prerequisites
- C++14 compatible compiler
- CMake 3.10 or higher
- Google Test framework
- Git (for version control)

### Installation

#### macOS
```bash
# Install prerequisites
xcode-select --install
brew install cmake

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
sudo apt-get install build-essential cmake libgtest-dev

# Clone and build
git clone <repository-url>
cd <repository-name>
mkdir build && cd build
cmake ..
make
```

#### Windows
```bash
# Install Visual Studio with C++ development tools
# Or use MinGW-w64

# Clone and build
git clone <repository-url>
cd <repository-name>
mkdir build && cd build
cmake -G "Visual Studio 16 2019" ..
cmake --build .
```

### Quick Start
```cpp
#include "arraySequence.hpp"
#include "listSequence.hpp"

// Create a dynamic array sequence
ArraySequence<int> arraySeq;
arraySeq.append(1);
arraySeq.append(2);
arraySeq.append(3);

// Create a linked list sequence
ListSequence<int> listSeq;
listSeq.append(4);
listSeq.append(5);
listSeq.append(6);

// Use iterators
for(auto it = arraySeq.begin(); it != arraySeq.end(); ++it) {
    std::cout << *it << " ";
}
```

### More Detailed Examples
```cpp
// Using SegmentedDeque with custom types
#include "segmentedDeque.hpp"
#include "types/complex.hpp"

SegmentedDeque<Complex> deque(16); // Create with segment size 16
deque.append(Complex(1, 2));
deque.prepend(Complex(0, 1));

// Immutable operations
auto newDeque = deque.appendImmutable(Complex(3, 4));

// Using with standard algorithms
std::vector<Complex> vec;
std::copy(deque.begin(), deque.end(), std::back_inserter(vec));

// Range-based for loop
for (const auto& item : deque) {
    std::cout << item << " ";
}

// Functional operations
// Map/Apply - transform elements
std::vector<double> magnitudes;
deque.apply(deque.begin(), deque.end(), 
           std::back_inserter(magnitudes),
           [](const Complex& c) { return c.magnitude(); });

// Where - filter elements
auto filtered = deque.where([](const Complex& c) { 
    return c.magnitude() > 2.0; 
});

// Reduce - aggregate values
double sum = deque.reduce([](double acc, const Complex& c) {
    return acc + c.magnitude();
}, 0.0);

// Sort elements
deque.sort(deque.begin(), deque.end());

```

## Data Flow
The library implements a layered architecture where sequence operations are transformed through the appropriate data structure implementation.

```ascii
[Client Code] -> [Sequence Interface]
                      |
                      v
    +----------------+----------------+
    |                |               |
[ArraySequence] [ListSequence] [SegmentedDeque]
    |                |               |
[DynamicArray] [LinkedList]    [Hybrid Storage]
```

Key component interactions:
1. All sequence implementations derive from the base Sequence template interface
2. ArraySequence uses DynamicArray for O(1) random access
3. ListSequence uses LinkedList for O(1) insertions
4. SegmentedDeque combines both approaches for balanced performance
5. Iterator implementations provide standard container interface
6. Exception handling ensures safe operation under invalid conditions
7. Template implementation allows for generic type support
8. Immutable operations create new instances without modifying original data