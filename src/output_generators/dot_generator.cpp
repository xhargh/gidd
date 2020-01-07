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

void DotGenerator::footer(ostream &os, bool clusters) {
  os << "}" << endl;
}

void DotGenerator::nodeDefinitionBegin(ostream &os, bool clusters) {
  subgraphs.clear();
}

void DotGenerator::nodeDefinition(ostream &os, shared_ptr<File> file, bool clusters) {
  this->subgraphs[file->path].insert(file);
}

void DotGenerator::nodeDefinitionEnd(ostream &os, bool clusters) {
  int clusterNum = 0;
  for (auto &cluster : subgraphs) {
    if (clusters) {
      os << "subgraph cluster_" << clusterNum << "{" << endl;
      os << "\tlabel = \"" << cluster.first << "\";" << endl;
    }
    for (auto &file : cluster.second) {
      string headerModifier = file->isHeader() ? "" : ", shape=box, style=filled, color=lightblue";
      os << "\t" << "\"" << file->FullPath() << "\" [ label=\"" << file->name << "\"" << headerModifier << "];" << endl;
    }
    if (clusters) {
      os << "}" << endl;
    }
    clusterNum++;

  }
}


void DotGenerator::edgeDefinition(ostream &os, shared_ptr<File> src, shared_ptr<File> dst, bool clusters) {
  string linkModifier=" [overlap=scale]";
  os << "\t" << "\"" << src->FullPath() << "\" -> \"" << dst->FullPath() << "\"" << linkModifier << ";" << endl;
}

