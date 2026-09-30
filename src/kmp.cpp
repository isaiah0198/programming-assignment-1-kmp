#include "kmp.hpp"

std::vector<int> buildLPS(const std::string& pattern) {
    std::vector<int> lps(pattern.size(), 0);

    int len = 0;
    size_t i = 1;

    while (i < pattern.size()) {
        if (pattern[i] == pattern[len]) {
            ++len;
            lps[i] = len;
            ++i;
        }
        else if (len > 0) {
            len = lps[len - 1];
        }
        else {
            lps[i] = 0;
            ++i;
        }
    }

    return lps;
}

// TODO: your next task
int kmpSearch(const std::string& text, const std::string& pattern) {
    return -1;
}

// TODO: after kmpSearch
int naiveSearch(const std::string& text, const std::string& pattern) {
    return -1;
}
