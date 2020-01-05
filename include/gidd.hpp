/**
 * Copyright Xhargh 2016-2020
 */

#ifndef GIDD_HPP
#define GIDD_HPP

#include <string>
#include <vector>
#include <set>
#include <map>

struct File {
  std::string name;
  std::string path;
  std::set<std::shared_ptr<File>> includes;
public:
  File(std::string name, std::string path) : name(name), path(path) {}
  static std::string FullPath(std::string path, std::string name) {
    return (path.length() ? (path + "/") : "") + name;
  }
  std::string FullPath() const {
    return File::FullPath(path, name);
  }
  bool operator<(const File& rhs) const
  {
    return FullPath().compare(rhs.FullPath());
  }
};

void generatePlantUml(
  const std::string &outputFile,
  const std::vector<std::string> &filters,
  const std::map<std::string, std::shared_ptr<File>> &nameToFileMap,
  std::map<std::string, std::set<std::string>> &paths,
  bool clusters = true);

#endif // GIDD_HPP
