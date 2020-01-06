/**
 * Copyright Xhargh 2016-2020
 */

#include "plantuml_generator.hpp"
#include "gidd_utils.hpp"

using namespace std;

PlantUmlGenerator::PlantUmlGenerator() : OutputGenerator(".puml") {

}

void PlantUmlGenerator::header(ostream& os, bool clusters) {

  os << "@startuml" << endl;
  if (clusters) {
    os << "\tset namespaceSeparator ::" << endl;
  }
  else {
    os << "\tset namespaceSeparator none" << endl;
  }
  os << "\thide members" << endl;
}

void PlantUmlGenerator::edgeDefinition(ostream& os, shared_ptr<File> src, shared_ptr<File> dst, bool clusters) {
  string to = dst->FullPath();
  string from = src->FullPath();

  if (clusters) {
    to = ReplaceString(to, "/", "::");
    from = ReplaceString(from, "/", "::");
  }

  os << "\t" << "\"" << to << "\"" << " <-- " << "\"" << from << "\"" << endl;
}

void PlantUmlGenerator::nodeDefinition(ostream& os, shared_ptr<File> file, bool clusters) {
  string filePath = file->FullPath();
  if (clusters) {
    filePath = ReplaceString(filePath, "/", "::");
  }
  os << "\tclass \"" << filePath << "\" " << (file->isHeader() ? "<< (h,lightgreen) >>" : "") << endl;
}

void PlantUmlGenerator::footer(ostream& os) {
  os << "@enduml" << endl;
}
