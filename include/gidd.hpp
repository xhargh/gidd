/**
 * Copyright Xhargh 2016-2020
 */

#ifndef GIDD_HPP
#define GIDD_HPP

#include <string>
#include <vector>
#include <set>
#include <map>
#include "gidd_utils.hpp"

struct File {
  std::string name;
  std::string path;
  std::set<std::shared_ptr<File>> includes;
public:
  File(std::string name, std::string path) : name(name), path(path) {}
  static std::string FullPath(const std::string path, const std::string name) {
    return (path.length() ? (path + "/") : "") + name;
  }
  std::string FullPath() const {
    return File::FullPath(path, name);
  }
  bool operator<(const File& rhs) const
  {
    return FullPath().compare(rhs.FullPath()) < 0;
  }
  bool isHeader() const {
    std::string file = FullPath();
    size_t lastdot = file.find_last_of(".");
    return lastdot == std::string::npos || file.find("h", lastdot) != std::string::npos ||
           file.find("H", lastdot) != std::string::npos;
  }
};

using node_t = std::shared_ptr<File>;
using edge_t = std::pair<node_t, node_t>;
using nodes_t = std::set<node_t>;
using edges_t = std::set<edge_t>;

struct graph_t {
  nodes_t nodes;
  edges_t edges;
};

int gidd();

#endif // GIDD_HPP
