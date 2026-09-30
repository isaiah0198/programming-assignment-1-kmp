#include <iostream>
#include <string>
#include "kmp.hpp"

int main() {
    // Temporary check: should print 0 1 0 1 2 2 3
    for (int v : buildLPS("AABAAAB")) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    while (true) {
        std::string text, pattern;

        std::cout << "Enter the text (or 'exit' to quit): ";
        std::getline(std::cin, text);

        if (text == "exit") {
            break;
        }

        std::cout << "Enter the pattern: ";
        std::getline(std::cin, pattern);

        int kmpResult = kmpSearch(text, pattern);
        int naiveResult = naiveSearch(text, pattern);

        std::cout << "KMP Search Result: " << kmpResult << std::endl;
        std::cout << "Naive Search Result: " << naiveResult << std::endl;
    }

    return 0;
}
