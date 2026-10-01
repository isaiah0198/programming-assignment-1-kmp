#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "kmp.hpp"

std::mt19937 rng(42); // fixed seed so results are reproducible

std::string randomText(int n, int alphabet) {
    std::uniform_int_distribution<int> dist(0, alphabet - 1);
    
    std::string s;
    s.reserve(n);

    for (int i = 0; i < n; ++i) {
        s.push_back('A' + dist(rng));
    }

    return s;
}

std::string repetitiveText(int n, char fill, char last) {
    if (n <= 0) {
        return "";
    }

    std::string s(n, fill);
    s[n - 1] = last;

    return s;
}

volatile int sink;

template <typename SearchFn>
long long timeSearch(SearchFn search, const std::string& text, const std::string& pattern, int runs) {
    
    std::vector<long long> times;
    for (int r = 0; r < runs; ++r) {
        auto start = std::chrono::steady_clock::now();

        sink = search(text, pattern);

        auto end = std::chrono::steady_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        times.push_back(duration.count());
    }

    std::sort(times.begin(), times.end());

    return times[times.size() / 2];
}

int main() {
    std::cout << "algorithm,experiment,text_size,pattern_size,input_type,alphabet_size,time_ns\n";

    std::vector<int> sizes = { 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000 };
    int runs = 51;

    for (int size : sizes) {
        std::string text = randomText(size, 26);
        std::string pattern = randomText(20, 26);

        int kmpResult = kmpSearch(text, pattern);
        int naiveResult = naiveSearch(text, pattern);

        long long kmpTime = timeSearch(kmpSearch, text, pattern, runs);
        long long naiveTime = timeSearch(naiveSearch, text, pattern, runs);

        if (kmpResult != naiveResult) {
                std::cerr << "Random text, size: " << size << ", naive: " << naiveResult << ", kmp: " << kmpResult << "\n";
                return 1;
        }

        std::cout << "KMP,text_size,"
          << size << ",20,random,26,"
          << kmpTime << '\n';

        std::cout << "Naive,text_size,"
          << size << ",20,random,26,"
          << naiveTime << '\n';
    }

    return 0;
}