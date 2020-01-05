/**
 * Copyright Xhargh 2016-2020
 */

#include <fstream>
#include <iostream>
#include "gidd.hpp"
#include "gidd_utils.hpp"

int main() {
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
  // set<pair<string, string>> pairs;
  map<string, string> fromFileToPath;

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
    fromFileToPath[key] = path;

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

  // generateDot(outputFile, filters, fileMap, fromFileToPath, paths, true);
  // generateDot(outputFile + "_no_clusters", filters, fileMap, fromFileToPath, paths, false);
  generatePlantUml(outputFile, filters, fileMap, paths, true);
  generatePlantUml(outputFile + "_no_clusters", filters, fileMap, paths, false);

  return 0;
}

