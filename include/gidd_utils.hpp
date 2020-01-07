/**
 * Copyright Xhargh 2016-2020
 */

#ifndef GIDD_UTILS_HPP
#define GIDD_UTILS_HPP

#include <string>
#include <vector>

#include "gidd.hpp" // just to test recursive includes

std::vector<std::string> split(const std::string &s, char delimiter);
std::string ReplaceString(std::string subject, const std::string& search, const std::string& replace);
std::string removeDots(const std::string input);

#endif // GIDD_UTILS_HPP
