# KMP String Search

A C++17 implementation of the Knuth–Morris–Pratt (KMP) string-search algorithm with a naive search baseline, automated tests, and an empirical benchmarking harness.

## Overview

This project was created for a programming assignment focused on implementing a non-trivial algorithm and carrying out an empirical study.

The project compares:

- **KMP search**, which uses a Longest Proper Prefix which is also a Suffix (LPS) table to avoid repeating work after partial matches.
- **Naive search**, which checks the pattern from each possible starting position in the text.

The benchmark investigates how both algorithms behave on:

- Random text generated from a 26-character alphabet.
- Highly repetitive text designed to create long partial matches.

## Project Structure

```text
.
├── benchmark/
│   └── benchmark.cpp
├── results/
│   ├── results.csv
│   ├── runtime_random.png
│   ├── runtime_repetitive.png
│   └── speedup.png
├── scripts/
│   └── plot_results.py
├── src/
│   ├── kmp.cpp
│   ├── kmp.hpp
│   └── main.cpp
├── tests/
│   └── test_kmp.cpp
├── CMakeLists.txt
└── README.md
```

## Requirements

- C++17-compatible compiler
- CMake 3.16 or later
- Python 3, only if regenerating plots
- Python packages:
  - `pandas`
  - `matplotlib`

The project was developed and benchmarked using:

- Apple Clang 21.0.0
- CMake 4.4.2
- MacBook Air with Apple M4 and 16 GB RAM

## Build

From the repository root:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Run the Interactive Search Program

```bash
./build/kmp
```

The program asks for a text and a pattern, then reports the result from both KMP and naive search.

Example:

```text
Enter the text (or 'exit' to quit): hello world
Enter the pattern: world
KMP Search Result: 6
Naive Search Result: 6
```

Both search functions return:

- the zero-based starting index if a match is found;
- `-1` if the pattern is not found;
- `0` for an empty pattern.

## Run the Tests

After building:

```bash
ctest --test-dir build --output-on-failure
```

The test suite covers cases including:

- match at the beginning;
- match in the middle;
- match at the end;
- no match;
- repeated characters;
- empty pattern;
- pattern longer than the text;
- overlapping partial matches such as `AAAB` with pattern `AAB`.

## Run the Benchmark

```bash
./build/benchmark
```

To save benchmark output as CSV:

```bash
./build/benchmark > results/results.csv
```

Each search is timed 51 times using `std::chrono::steady_clock`, and the median runtime is recorded.

Random benchmark inputs use `std::mt19937` with a fixed seed of `42` so the generated data is reproducible.

Before timing an input, the benchmark checks that KMP and naive search return the same result.

## Benchmark Experiments

### Experiment 1: Random Text

- Alphabet size: 26 uppercase characters
- Pattern length: 20
- Text sizes: 1,000 to 10,000,000 characters

This experiment examines how both algorithms scale on random input.

### Experiment 2: Repetitive Text

- Text structure: `AAAA...AB`
- Pattern structure: `AAAA...AB`
- Pattern length: 20
- Text sizes: 1,000 to 1,000,000 characters

This experiment is designed to create long partial matches and expose the repeated work performed by naive search.

## Generate Plots

Install the Python dependencies if required:

```bash
python3 -m pip install pandas matplotlib
```

Then run:

```bash
python3 scripts/plot_results.py
```

The script generates:

```text
results/runtime_random.png
results/runtime_repetitive.png
results/speedup.png
```

## Main Functions

### `buildLPS`

```cpp
std::vector<int> buildLPS(const std::string& pattern);
```

Builds the LPS table used by KMP.

### `kmpSearch`

```cpp
int kmpSearch(const std::string& text, const std::string& pattern);
```

Searches for the first occurrence of `pattern` in `text` using KMP.

### `naiveSearch`

```cpp
int naiveSearch(const std::string& text, const std::string& pattern);
```

Searches for the first occurrence of `pattern` by testing each possible starting position.