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

void generatePlantUml(
  const string &outputFile,
  const vector<string> &filters,
  const map<string, shared_ptr<File>> &nameToFileMap,
  bool clusters)
{
  filebuf fb;
  fb.open (outputFile + ".puml", ios_base::out);
  ostream os(&fb);

  os << "@startuml" << endl;
  if (clusters) {
    os << "\tset namespaceSeparator ::" << endl;
  }
  else {
    os << "\tset namespaceSeparator none" << endl;
  }
  os << "\thide members" << endl;

  for (auto &a : nameToFileMap) {
    bool matchFilter = checkPathFilter(a.second->FullPath(), filters);
    if (!matchFilter) {
      for (auto &b : a.second->includes) {
        string file = b->FullPath();
        if (!checkPathFilter(file, filters)) {
          if (clusters) {
            file = ReplaceString(file, "/", "::");
          }
          os << "\tclass \"" << file << "\" " << (b->isHeader() ? "<< (h,lightgreen) >>" : "") << endl;
        }
      }
    }
  }

  for (auto& includer: nameToFileMap) {
    auto includerPath = includer.second->FullPath();
    if (!checkPathFilter(includerPath, filters)) {
      string from = ReplaceString(includerPath, "/", "::");
      for (auto &includee: includer.second->includes) {
        auto includeePath = includee->FullPath();
        if (!checkPathFilter(includeePath, filters)) {
          string to = includeePath;
          if (clusters) {
            to = ReplaceString(to, "/", "::");
          }
          os << "\t" << "\"" << to << "\"" << " <-- " << "\"" << from << "\"" << endl;
        }
      }
    }
  }

  os << "@enduml" << endl;

  fb.close();
}

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

  for (auto& x : fileMap) {
    cout << x.second->name << " -> " << endl;
    for (auto& y: x.second->includes) {
      cout << "\t" << y->name << endl;
    }
    cout << endl;
  }

  // generateDot(outputFile, filters, fileMap, fromFileToPath, paths, true);
  // generateDot(outputFile + "_no_clusters", filters, fileMap, fromFileToPath, paths, false);
  generatePlantUml(outputFile, filters, fileMap, true);
  generatePlantUml(outputFile + "_no_clusters", filters, fileMap, false);

  return 0;
}