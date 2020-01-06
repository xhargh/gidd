/**
 * Copyright Xhargh 2016-2020
 */


#ifndef GIDD_FILE_HPP
#define GIDD_FILE_HPP

#include <string>
#include <set>

struct File {
  std::string name;
  std::string path;
  std::set<std::shared_ptr<File>> includes;
public:
  File(std::string name, std::string path);
  static std::string FullPath(const std::string path, const std::string name);
  std::string FullPath() const;
  bool operator<(const File& rhs) const;
  bool isHeader() const;
};

#endif //GIDD_FILE_HPP
