/**
 * Copyright Xhargh 2016-2020
 */

#ifndef GIDD_PLANTUML_GENERATOR_HPP
#define GIDD_PLANTUML_GENERATOR_HPP

#include "output_generator.hpp"

class PlantUmlGenerator : public OutputGenerator {
public:
  PlantUmlGenerator();

  void header(std::ostream& os, bool clusters) override;

  void edgeDefinition(std::ostream& os, std::shared_ptr<File> src, std::shared_ptr<File> dst, bool clusters) override;

  void nodeDefinition(std::ostream& os, std::shared_ptr<File> file, bool clusters) override;

  void footer(std::ostream& os) override;
};



#endif //GIDD_PLANTUML_GENERATOR_HPP
