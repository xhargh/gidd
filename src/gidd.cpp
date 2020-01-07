/**
 * Copyright Xhargh 2016-2020
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include "gidd.hpp"
#include "dot_generator.hpp"
#include "plantuml_generator.hpp"
#include "gml_generator.hpp"

using namespace std;


// TODO: add two kinds of filtering:
//       a: remove all edges where any of the files is in the filter list
//       b: only remove all edges away from a file in the filter list
// TODO: also, make sure that all includes are reachable from the compiled cpp files, otherwise we'll get cluster of files that are not relevant
// TODO: clean up beginning of paths if not needed (i.e. no duplicates)




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
      auto pathStart = l.find("object ") + 7;
      auto pathEnd = l.find_last_of("/");
      path = removeDots(l.substr(pathStart, pathEnd-pathStart));
      file = l.substr(pathEnd + 1); // remove everything up to last /
      cout << path << endl;
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

  DotGenerator dotter;
  dotter.generate(outputFile, filters, files, true);
  dotter.generate(outputFile + "_no_clusters", filters, files, false);
  PlantUmlGenerator pumler;
  pumler.generate(outputFile, filters, files, true);
  pumler.generate(outputFile + "_no_clusters", filters, files, false);
  GmlGenerator gmler;
  gmler.generate(outputFile, filters, files, true);
  gmler.generate(outputFile + "_no_clusters", filters, files, false);

  return 0;
}