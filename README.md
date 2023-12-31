![Test Badge](https://github.com/WillisMedwell/wm/actions/workflows/test.yml/badge.svg) ![Benchmark Badge](https://github.com/WillisMedwell/wm/actions/workflows/benchmark.yml/badge.svg)
# wm
Header only libraries for everyday C++ use.  

- [wm](#wm)
    - [Sov (Structure of Vectors)](#sov-structure-of-vectors)
    - [Functors](#functors)
    - [SplitByElement](#splitbyelement)
- [Installation](#installation)
    - [Manual](#manual)
    - [CMake](#cmake)


### Sov (Structure of Vectors)
Like a structure of arrays but with reallocation like std::vector. All memory is in one block making it very cache friendly.

A live comparison between an *object orientated* approach and *data orientated* approach using std::vector vs wm::Sov can be found [here](https://willismedwell.github.io/wm/).

**Usage**
```cpp
wm::Sov<uint8_t, std::string, uint16_t> sov;

sov.pushBack(0, "first", 1);
sov.pushBack(0, "second", 2);

// get individual field (like using std::get<>)
for (auto e : sov.field<0>()) {
    std::cout << (int)e << ' ';
}
for (auto e : sov.field<uint16_t>()) {
    std::cout << (int)e << ' ';
}

// range based loop (takes it as a reference)
for (const auto& [a, b, c] : sov) {
    std::cout << "(" << (int)a << ", " << b << ", " << (int)c << ")\n";
}

// reference to elements
auto [field_1, field_2, field_3] = sov[2];
```

### Functors
Honestly cannot believe the std doesn't support these. Literally blows my mind.

**Usage**

```cpp
std::vector<int> range = { 1, 2, 3, 4, 5, 6};

std::ranges::find_if(range, wm::IsEqualTo(4));
std::ranges::find_if(range, wm::IsNotEqualTo(1));
std::ranges::any_of(range, wm::IsGreaterThan(5));
std::ranges::all_of(range, wm::IsLessThan(1));
```

### SplitByElement
Takes a contiguous range and returns spans into that range with no copying. 
- Range based loops = lazily evaluated.
- Indexing = on first index, are evaluated.

*std::views::split & std::views::lazy_split are too temperamental in my opinion.*

**Usage**

```c++
std::string_view sentence = " I went for a run yesterday ";
for(std::span<const char> word_data : wm::SplitByElement{sentence, ' ' })
{
    std::string_view word { word_data.begin(), word_data.end() };
    std::cout << word << ',';
}
// "I,went,for,a,run,yesterday,"
```

```c++
std::vector<int> nums = { 2, 1, 2, 2, 3, 4, 2 };

num_grouping = wm::SplitByElement{nums, 2};

std::span<int> num_group0 = num_grouping[0];
// [ 1 ]
std::span<int> num_group1 = num_grouping[1];
// [ 3, 4 ]
```

# Installation
### Manual
Download the headers from the [Releases](https://github.com/WillisMedwell/wm/releases/tag/Headers).

### CMake 
You can use the CMake fetching module to do it all for you.
```cmake
# Cmake's fetch module.
include(FetchContent)

# Fetch the ZIP file containing the headers. Lets you always have the latest release.
FetchContent_Declare(
  wm_headers
  URL https://github.com/WillisMedwell/wm/releases/download/Headers/headers.zip
)

FetchContent_GetProperties(wm_headers)
FetchContent_Populate(wm_headers)
set(WM_HEADERS_DIR ${wm_headers_SOURCE_DIR}) 
```
```cmake
# Add to your include list.
target_include_directories(${CMAKE_PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${WM_HEADERS_DIR}
)
```
Then using it is as simple as...
```c++
#include "wm/Sov.hpp"

int main()
{
    wm::Sov<int, int, int> foos; 
}
```