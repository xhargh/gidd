/**
 * Copyright Xhargh 2016-2020
 */

#include "output_generator.hpp"
#include <fstream>

using namespace std;

static bool checkPathFilter(const string a, const vector<string> &filters) {
  for (auto &f : filters) {
    if (a.substr(0, f.size()) == f) {
      return true;
    }
  }
  return false;
}


graph_t OutputGenerator::generateNodesAndEdges(const std::set<std::shared_ptr<File>> &files, const std::vector<std::string> &filters) const {
  nodes_t nodes;
  edges_t edges;

  for (auto &includer : files) {
    auto includerPath = includer->FullPath();
    if (!checkPathFilter(includerPath, filters)) {

      if (!includer->includes.empty()) {
        nodes.insert(includer);
      }

      for (auto &includee : includer->includes) {
        if (!checkPathFilter(includee->FullPath(), filters)) {
          nodes.insert(includee);
          edges.insert(make_pair(includer, includee));
        }
      }

    }
  }
  return {nodes, edges};
}


void OutputGenerator::generate(
  const std::string &outputFile,
  const std::vector<std::string> &filters,
  const std::set<std::shared_ptr<File>> &files,
  bool clusters)
{

  graph_t graph = generateNodesAndEdges(files, filters);


  filebuf fb;
  fb.open (outputFile + extension, ios_base::out);
  ostream os(&fb);

  header(os, clusters);

  nodeDefinitionBegin(os);
  for (auto &node : graph.nodes) {
    nodeDefinition(os, node, clusters);
  }
  nodeDefinitionEnd(os);

  edgeDefinitionBegin(os);
  for (auto &edge: graph.edges) {
    edgeDefinition(os, edge.first, edge.second, clusters);
  }
  edgeDefinitionEnd(os);

  footer(os);

  fb.close();
}