/**
 * Copyright Xhargh 2016-2020
 */


#ifndef GIDD_GRAPH_HPP
#define GIDD_GRAPH_HPP

#include <memory>
#include <file.hpp>

using node_t = std::shared_ptr<File>;
using edge_t = std::pair<node_t, node_t>;
using nodes_t = std::set<node_t>;
using edges_t = std::set<edge_t>;

struct graph_t {
  nodes_t nodes;
  edges_t edges;
};


#endif //GIDD_GRAPH_HPP
