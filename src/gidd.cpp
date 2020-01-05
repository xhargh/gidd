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





bool checkPathFilter(const string a, const vector<string> &filters) {
  for (auto &f : filters) {
    if (a.substr(0, f.size()) == f) {
      return true;
    }
  }
  return false;
}

bool checkPairs(
  const shared_ptr<File>firstFile,
  const shared_ptr<File>secondFile,
  const vector<string> &filters) {

  auto firstFileFullPath = firstFile->FullPath();
  auto secondFileFullPath = secondFile->FullPath();

  for (auto &f : filters)
  {
    if (
      (firstFileFullPath.substr(0, f.size()) == f)
      ||
      (secondFileFullPath.substr(0, f.size()) == f)
      ) {
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
  map<string, set<string>> &paths,
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

        // TODO: add two kinds of filtering:
        //       a: remove all edges where any of the files is in the filter list
        //       b: only remove all edges away from a file in the filter list
        // TODO: also, make sure that all includes are reachable from the compiled cpp files, otherwise we'll get cluster of files that are not relevant
        if (!checkPathFilter(file, filters)) {

          bool isHeader = false;
          size_t lastdot = file.find_last_of(".");
          if (lastdot == string::npos || file.find("h", lastdot) != string::npos ||
              file.find("H", lastdot) != string::npos) {
            isHeader = true;
          }

          if (clusters) {
            file = ReplaceString(file, "/", "::");
          }

          os << "\tclass \"" << file << "\" " << (isHeader ? "<< (h,lightgreen) >>" : "") << endl;
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
