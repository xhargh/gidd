/**
 * Copyright Xhargh 2016-2020
 */


#ifndef GIDD_OUTPUT_GENERATOR_HPP
#define GIDD_OUTPUT_GENERATOR_HPP

#include <string>
#include <ostream>
#include <set>
#include <vector>
#include "file.hpp"
#include "graph.hpp"

class OutputGenerator {
protected:
  const std::string extension;
  OutputGenerator(const std::string extension) : extension(extension) {}
  virtual void header(std::ostream& os, bool clusters) {};
  virtual void footer(std::ostream& os, bool clusters) {};
  virtual void nodeDefinitionBegin(std::ostream& os, bool clusters) {};
  virtual void nodeDefinition(std::ostream& os, std::shared_ptr<File> file, bool clusters) {};
  virtual void nodeDefinitionEnd(std::ostream& os, bool clusters) {};
  virtual void edgeDefinitionBegin(std::ostream& os, bool clusters) {};
  virtual void edgeDefinition(std::ostream& os, std::shared_ptr<File> src, std::shared_ptr<File> dst, bool clusters) {};
  virtual void edgeDefinitionEnd(std::ostream& os, bool clusters) {};

  graph_t generateNodesAndEdges(const std::set<std::shared_ptr<File>> &files, const std::vector<std::string> &filters) const;
public:
  void generate(
    const std::string &outputFile,
    const std::vector<std::string> &filters,
    const std::set<std::shared_ptr<File>> &files,
    bool clusters);

};

#endif //GIDD_OUTPUT_GENERATOR_HPP
