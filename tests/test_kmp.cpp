#include <cassert>
#include <iostream>

#include "kmp.hpp"

int main()
{
    // Match at beginning
    assert(kmpSearch("hello", "he") == 0);
    assert(naiveSearch("hello", "he") == 0);

    // Match in middle
    assert(kmpSearch("hello world", "world") == 6);
    assert(naiveSearch("hello world", "world") == 6);

    // Match at end
    assert(kmpSearch("abcdef", "def") == 3);
    assert(naiveSearch("abcdef", "def") == 3);

    // No match
    assert(kmpSearch("abcdef", "xyz") == -1);
    assert(naiveSearch("abcdef", "xyz") == -1);

    // Pattern equals text
    assert(kmpSearch("abcdef", "abcdef") == 0);
    assert(naiveSearch("abcdef", "abcdef") == 0);

    // Repeated characters
    assert(kmpSearch("aaaaaa", "aaa") == 0);
    assert(naiveSearch("aaaaaa", "aaa") == 0);

    // Pattern longer than text
    assert(kmpSearch("abc", "abcdef") == -1);
    assert(naiveSearch("abc", "abcdef") == -1);

    // Empty pattern
    assert(kmpSearch("abc", "") == 0);
    assert(naiveSearch("abc", "") == 0);

    // Single-character match
    assert(kmpSearch("abc", "b") == 1);
    assert(naiveSearch("abc", "b") == 1);

    // KMP-style overlapping/repetitive case
    assert(kmpSearch("ababababac", "ababac") == 4);
    assert(naiveSearch("ababababac", "ababac") == 4);

    // KMP fallback: mismatch with j > 0 must not advance i
    assert(kmpSearch("AAAB", "AAB") == 4);
    assert(naiveSearch("AAAB", "AAB") == 4);

    // Empty text
    assert(kmpSearch("", "a") == -1);
    assert(naiveSearch("", "a") == -1);

    // Empty text & Empty pattern
    assert(kmpSearch("", "") == 0);
    assert(naiveSearch("", "") == 0);

    std::cout << "All tests passed.\n";
    return 0;
}