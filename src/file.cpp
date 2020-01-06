/**
 * Copyright Xhargh 2016-2020
 */

#include "./././file.hpp"

File::File(std::string name, std::string path) : name(name), path(path) {}

std::string File::FullPath(const std::string path, const std::string name) {
  return (path.length() ? (path + "/") : "") + name;
}

std::string File::FullPath() const {
  return File::FullPath(path, name);
}

bool File::operator<(const File& rhs) const
{
  return FullPath().compare(rhs.FullPath()) < 0;
}

bool File::isHeader() const {
  std::string file = FullPath();
  size_t lastdot = file.find_last_of(".");
  return lastdot == std::string::npos || file.find("h", lastdot) != std::string::npos ||
         file.find("H", lastdot) != std::string::npos;
}


