/**
 * Copyright Xhargh 2016-2020
 */

#ifndef GIDD_DOT_GENERATOR_HPP
#define GIDD_DOT_GENERATOR_HPP

#include "output_generator.hpp"
#include <map>
#include <string>
#include <ostream>


class DotGenerator : public OutputGenerator {
public:
  DotGenerator();
private:
  std::map<std::string, nodes_t> subgraphs;

protected:
  void header(std::ostream &os, bool clusters) override;

  void footer(std::ostream &os, bool clusters) override;

  void nodeDefinitionBegin(std::ostream &os, bool clusters) override;

  void nodeDefinition(std::ostream &os, std::shared_ptr<File> file, bool clusters) override ;

  void nodeDefinitionEnd(std::ostream &os, bool clusters) override;

  void edgeDefinition(std::ostream &os, std::shared_ptr<File> src, std::shared_ptr<File> dst, bool clusters) override;

};


#endif //GIDD_DOT_GENERATOR_HPP
