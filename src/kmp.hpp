#pragma once
#include <string>
#include <vector>

std::vector<int> buildLPS(const std::string& pattern);
int kmpSearch(const std::string& text, const std::string& pattern);
int naiveSearch(const std::string& text, const std::string& pattern);
