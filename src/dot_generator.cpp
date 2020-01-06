/**
 * Copyright Xhargh 2016-2020
 */

#include "dot_generator.hpp"

using namespace std;

DotGenerator::DotGenerator() : OutputGenerator(".dot") {

}

void DotGenerator::header(ostream &os, bool clusters) {
  os << "digraph G {" << endl;
}

void DotGenerator::footer(ostream &os) {
  os << "}" << endl;
}

void DotGenerator::nodeDefinitionBegin(ostream &os) {
  clusters.clear();
}

void DotGenerator::nodeDefinition(ostream &os, shared_ptr<File> file, bool clusters) {
  this->clusters[file->path].insert(file);
  useClusters = clusters;
}

void DotGenerator::nodeDefinitionEnd(ostream &os) {
  int clusterNum = 0;
  for (auto &cluster : clusters) {
    if (useClusters) {
      os << "subgraph cluster_" << clusterNum << "{" << endl;
      os << "\tlabel = \"" << cluster.first << "\";" << endl;
    }
    for (auto &file : cluster.second) {
      string headerModifier = file->isHeader() ? "" : ", shape=box, style=filled, color=lightblue";
      os << "\t" << "\"" << file->FullPath() << "\" [ label=\"" << file->name << "\"" << headerModifier << "];" << endl;
    }
    if (useClusters) {
      os << "}" << endl;
    }
    clusterNum++;

  }
}


void DotGenerator::edgeDefinition(ostream &os, shared_ptr<File> src, shared_ptr<File> dst, bool clusters) {
  string linkModifier=" [overlap=scale]";
  os << "\t" << "\"" << src->FullPath() << "\" -> \"" << dst->FullPath() << "\"" << linkModifier << ";" << endl;
}

