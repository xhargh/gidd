/**
 * Copyright Xhargh 2016-2020
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include "gidd.hpp"
#include "../include/gidd_utils.hpp"

using namespace std;


// TODO: add two kinds of filtering:
//       a: remove all edges where any of the files is in the filter list
//       b: only remove all edges away from a file in the filter list
// TODO: also, make sure that all includes are reachable from the compiled cpp files, otherwise we'll get cluster of files that are not relevant
// TODO: clean up beginning of paths if not needed (i.e. no duplicates)

bool checkPathFilter(const string a, const vector<string> &filters) {
  for (auto &f : filters) {
    if (a.substr(0, f.size()) == f) {
      return true;
    }
  }
  return false;
}



#if 0
void generateDot(
  const string &outputFile,
  const vector<string> &filters,
  const map<string, shared_ptr<File>> &pairs,
  map<string, string> &fromFileToPath,
  map<string, set<string>> &paths,
  bool clusters = true)
{
  filebuf fb;
  fb.open (outputFile + ".dot", ios_base::out);
  ostream os(&fb);

  os << "digraph G {" << endl;

  if (clusters) {
    int clusterNum = 0;

    for (auto &a : paths) {
      bool matchFilter = checkPathFilter(a.first, filters);

      if (!matchFilter) {
        os << "subgraph cluster_" << clusterNum << "{" << endl;
        os << "label = \"" << a.first << "\";" << endl;
        for (auto &b : a.second) {
          os << "\"" << b << "\";" << endl;
        }
        os << "}" << endl;
        clusterNum++;
      }
    }
  }

  for (auto& p : pairs)
  {
    bool matchFilter = checkPairs(p, fromFileToPath, filters);

    if (!matchFilter) {
      string headerModifier = " [shape=box, style=filled, color=lightblue]";
      string linkModifier=" [overlap=scale]";

      if (fromFileToPath[p.first].compare("") == 0)
      {
        os << "\t" << "\"" << p.first << "\"" << headerModifier << ";" << endl;
      }
      os << "\t" << "\"" << p.first << "\"" << " -> "  << "\"" << p.second << "\"" << linkModifier << ";" << endl;
    }
  }

  os << "}" << endl;

  fb.close();
}
#endif

class OutputGenerator {
protected:
  const string extension;
  OutputGenerator(const string extension) : extension(extension) {}
  virtual void header(ostream& os, bool clusters) const {};
  virtual void footer(ostream& os) const {};
  virtual void nodeDefinitionBegin(ostream& os) const {};
  virtual void nodeDefinition(ostream& os, shared_ptr<File> file, bool clusters) const {};
  virtual void nodeDefinitionEnd(ostream& os) const {};
  virtual void edgeDefinitionBegin(ostream& os) const {};
  virtual void edgeDefinition(ostream& os, shared_ptr<File> src, shared_ptr<File> dst, bool clusters) const {};
  virtual void edgeDefinitionEnd(ostream& os) const {};

  graph_t generateNodesAndEdges(const set<shared_ptr<File>> &files, const vector<string> &filters) const {
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
public:
  void generate(
    const string &outputFile,
    const vector<string> &filters,
    const set<shared_ptr<File>> &files,
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

};

class DotGenerator : public OutputGenerator {
public:
  DotGenerator() : OutputGenerator(".dot") {}
protected:
  void header(ostream &os, bool clusters) const override {
    os << "digraph G {" << endl;
  }

  void footer(ostream &os) const override {
    os << "}" << endl;
  }

  void nodeDefinition(ostream &os, shared_ptr<File> file, bool clusters) const override {
    string headerModifier = file->isHeader() ? "" : " [shape=box, style=filled, color=lightblue]";
    os << "\t" << "\"" << file->name << "\"" << headerModifier << ";" << endl;
  }

  void edgeDefinition(ostream &os, shared_ptr<File> src, shared_ptr<File> dst, bool clusters) const override {
    string linkModifier=" [overlap=scale]";
    os << "\t" << "\"" << src->name << "\" -> \"" << dst->name << "\"" << linkModifier << ";" << endl;
  }

  void nodeDefinitionBegin(ostream &os) const override {
  }

  void nodeDefinitionEnd(ostream &os) const override {
  }

  void edgeDefinitionBegin(ostream &os) const override {
  }

  void edgeDefinitionEnd(ostream &os) const override {
  }

};

class PlantUmlGenerator : public OutputGenerator {
public:
  PlantUmlGenerator() : OutputGenerator(".puml") {

  }

  void header(ostream& os, bool clusters) const override {

    os << "@startuml" << endl;
    if (clusters) {
      os << "\tset namespaceSeparator ::" << endl;
    }
    else {
      os << "\tset namespaceSeparator none" << endl;
    }
    os << "\thide members" << endl;
  }

  void edgeDefinition(ostream& os, shared_ptr<File> src, shared_ptr<File> dst, bool clusters) const override {
    string to = dst->FullPath();
    string from = src->FullPath();

    if (clusters) {
      to = ReplaceString(to, "/", "::");
      from = ReplaceString(from, "/", "::");
    }

    os << "\t" << "\"" << to << "\"" << " <-- " << "\"" << from << "\"" << endl;
  }

  void nodeDefinition(ostream& os, shared_ptr<File> file, bool clusters) const override {
    string filePath = file->FullPath();
    if (clusters) {
      filePath = ReplaceString(filePath, "/", "::");
    }
    os << "\tclass \"" << filePath << "\" " << (file->isHeader() ? "<< (h,lightgreen) >>" : "") << endl;
  }

  void footer(ostream& os) const override {
    os << "@enduml" << endl;
  }

};



int gidd() {
  using namespace std;

  string line;
  vector<string> lines;

  const string inputFileName = "input.txt";
  const string filterFileName = "filter.txt";
  const string outputFile = "output"; // extension is added later

  // Read input
  ifstream infile(inputFileName);
  while (getline(infile, line))
  {
    auto firstchar = line.at(0);
    if (firstchar == '.'
        || (firstchar == '['
            && (
              line.find("Building CXX object") != string::npos || line.find("Building C object") != string::npos))) {
      lines.push_back(line);
    }
  }

  // Read filters
  vector<string> filters;
  ifstream filterFile(filterFileName);
  while (getline(filterFile, line))
  {
    filters.push_back(line);
  }

  vector<string> s;

  map<string, shared_ptr<File>> fileMap; // name -> File struct

  for (auto& l : lines)
  {
    size_t level = l.find(" ");
    string file;
    string path = "";

    if (l.at(0) == '[') {
      level = 0;

      file = l.substr(l.find_last_of("/")+1); // remove everything up to last /
      // TODO: add support for other extensions than .o
      file = file.substr(0, file.length()-2); // remove ".o"
    }
    else {
      string fullpath = l.substr(level + 1); // full path

      file = l.substr(l.find_last_of("/")+1); // only filename
      path = removeDots(fullpath.substr(0, fullpath.find_last_of("/"))); // only path
    }
    string key = File::FullPath(path, file);

    // Only insert new file if not already created
    if (fileMap.count(key) == 0) {
      fileMap[key] = make_shared<File>(file, path);
    }

    if (s.size() <= level)
    {
      s.push_back(key);
    }
    else {
      s[level] = key;
    }

    if (level > 0)
    {
      string includer = s[level-1];
      fileMap[includer]->includes.insert(fileMap[key]);
    }
  }


  set<shared_ptr<File>> files;
  for (auto& f : fileMap) {
    files.insert(f.second);
  }

  // generateDot(outputFile, filters, fileMap, fromFileToPath, paths, true);
  // generateDot(outputFile + "_no_clusters", filters, fileMap, fromFileToPath, paths, false);
  DotGenerator dotter;
  dotter.generate(outputFile, filters, files, true);
  dotter.generate(outputFile + "_no_clusters", filters, files, false);
  PlantUmlGenerator pumler;
  pumler.generate(outputFile, filters, files, true);
  pumler.generate(outputFile + "_no_clusters", filters, files, false);

  return 0;
}