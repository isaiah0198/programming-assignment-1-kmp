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

int kmpSearch(const std::string& text, const std::string& pattern) {
    if (pattern.empty()) {
        return 0;
    }

    std::vector<int> lps = buildLPS(pattern);

    // text index
    int i = 0;
    // pattern index
    int j = 0;

    while (i < text.size()) {
        if (text[i] == pattern[j]) {
            ++i;
            ++j;

            if (j == pattern.size()){
                return i - j;
            }
        }
        else if (j > 0) {
            j = lps[j - 1];
        }
        else {
            ++i;
        }
    }
    return -1;
}

int naiveSearch(const std::string& text, const std::string& pattern) {
    return -1;
}
