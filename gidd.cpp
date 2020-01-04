/**
 * Copyright Xhargh 2016-2020
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>

using namespace std;

string ReplaceString(string subject, const string& search,
                     const string& replace) {
  size_t pos = 0;
  while ((pos = subject.find(search, pos)) != string::npos) {
    subject.replace(pos, search.length(), replace);
    pos += replace.length();
  }
  return subject;
}

bool checkPathFilter(const string a, const vector<string> &filters) {
  for (auto &f : filters) {
    if (a.substr(0, f.size()) == f) {
      return true;
    }
  }
  return false;
}

bool checkPairs(
  const pair<string, string> &p,
  const map<string, string> &fromFileToPath,
  const vector<string> &filters) {

  auto firstPath = fromFileToPath.find(p.first);
  auto secondPath = fromFileToPath.find(p.second);

  for (auto &f : filters)
  {
    if (
      ((firstPath != fromFileToPath.end()) && (firstPath->second.substr(0, f.size()) == f))
      ||
      ((secondPath != fromFileToPath.end()) && (secondPath->second.substr(0, f.size()) == f))
      ) {
      return true;
    }
  }
  return false;
}

void generateDot(
  const string &outputFile,
  const vector<string> &filters,
  const set<pair<string, string>> &pairs,
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

void generatePlantUml(
  const string &outputFile,
  const vector<string> &filters,
  const set<pair<string, string>> &pairs,
  map<string, string> &fromFileToPath,
  map<string, set<string>> &paths,
  bool clusters = true)
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

  for (auto &a : paths) {
    bool matchFilter = checkPathFilter(a.first, filters);

    if (!matchFilter) {

      for (auto &b : a.second) {
        string file = b;

        bool isHeader = false;
        size_t lastdot = file.find_last_of(".");
        if (lastdot == string::npos || file.find("h", lastdot) != string::npos || file.find("H", lastdot) != string::npos)
        {
          isHeader = true;
        }

        if (clusters) {
          string path = fromFileToPath[file];
          string fullPath = path + "/" + file;
          file = ReplaceString(fullPath, "/", "::");
        }

        os << "\tclass \"" << file << "\" " << (isHeader?"<< (h,lightgreen) >>":"") << endl;
      }
    }
  }

  for (auto& p : pairs)
  {
    bool matchFilter = checkPairs(p, fromFileToPath, filters);

    if (!matchFilter) {
      string from = p.first;
      string to = p.second;
      if (clusters) {
        string fullPathFirst = fromFileToPath[p.first] + "/" + p.first;
        string fullPathSecond = fromFileToPath[p.second] + "/" + p.second;

        from = ReplaceString(fullPathFirst, "/", "::");
        to = ReplaceString(fullPathSecond, "/", "::");
      }

      os << "\t" << "\"" << to << "\"" << " <-- "  << "\"" << from << "\"" << endl;
    }
  }

  os << "@enduml" << endl;

  fb.close();
}

int main() {
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
  set<pair<string, string>> pairs;
  map<string, string> fromFileToPath;

  for (auto& l : lines)
  {
    size_t level = l.find(" ");
    string file;

    if (l.at(0) == '[') {
      level = 0;

      file = l.substr(l.find_last_of("/")+1); // remove everything up to last /
      // TODO: add support for other extensions than .o
      file = file.substr(0, file.length()-2); // remove ".o"

      fromFileToPath[file] = "";
    }
    else {
      string fullpath = l.substr(level + 1); // full path

      file = l.substr(l.find_last_of("/")+1); // only filename
      string path = fullpath.substr(0, fullpath.find_last_of("/")); // only path

      fromFileToPath[file] = path;
    }

    if (s.size() <= level)
    {
      s.push_back(file);
    }
    else {
      s[level] = file;
    }

    if (level > 0)
    {
      pairs.insert(pair<string, string>(s[level-1], s[level]));
    }
  }

  // Create a map from folder to set of files
  map<string, set<string>> paths;
  for (auto& fp : fromFileToPath)
  {
    paths[fp.second].insert(fp.first);
  }

  cout << "Include folders actually used:" << endl;
  for (auto& p : paths)
  {
    if (p.first.length()) { // don't print ""
      cout << p.first << endl;
    }
  }

  generateDot(outputFile, filters, pairs, fromFileToPath, paths, true);
  generateDot(outputFile + "_no_clusters", filters, pairs, fromFileToPath, paths, false);
  generatePlantUml(outputFile, filters, pairs, fromFileToPath, paths, true);
  generatePlantUml(outputFile + "_no_clusters", filters, pairs, fromFileToPath, paths, false);

  return 0;
}
