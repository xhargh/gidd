/**
 * Copyright Xhargh 2016-2020
 */

#ifndef GIDD_GML_GENERATOR_HPP
#define GIDD_GML_GENERATOR_HPP

#include "output_generator.hpp"
#include <map>


class GmlGenerator : public OutputGenerator {
private:
  std::map<node_t, int> nodeIds;
  std::map<std::string, int> clusterIds;
  int currentNodeId;
  int currentClusterId;
public:
  GmlGenerator();
protected:
  void header(std::ostream &os, bool clusters) override;

  void footer(std::ostream &os) override;

  void nodeDefinition(std::ostream &os, std::shared_ptr<File> file, bool clusters) override;

  void nodeDefinitionEnd(std::ostream &os) override;

  void edgeDefinition(std::ostream &os, std::shared_ptr<File> src, std::shared_ptr<File> dst, bool clusters) override;

};


#endif //GIDD_GML_GENERATOR_HPP
