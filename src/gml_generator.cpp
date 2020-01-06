/**
 * Copyright Xhargh 2016-2020
 */

/*
Example from wikipedia for Graph Modelling Language https://en.wikipedia.org/wiki/Graph_Modelling_Language

graph [
	comment "This is a sample graph"
	directed 1
	id 42
	label "Hello, I am a graph"
	node [
		id 1
		label "node 1"
		thisIsASampleAttribute 42
	]
	node [
		id 2
		label "node 2"
		thisIsASampleAttribute 43
	]
	node [
		id 3
		label "node 3"
		thisIsASampleAttribute 44
	]
	edge [
		source 1
		target 2
		label "Edge from node 1 to node 2"
	]
	edge [
		source 2
		target 3
		label "Edge from node 2 to node 3"
	]
	edge [
		source 3
		target 1
		label "Edge from node 3 to node 1"
	]
]

 */

#include "gml_generator.hpp"

using namespace std;

GmlGenerator::GmlGenerator() : OutputGenerator(".gml") {
}

void GmlGenerator::header(std::ostream &os, bool clusters) {
  os << "graph [" << endl;
  // os << "\t" << "comment \"This is a gidd graph\""
  os << "\t" << "directed 1" << endl;
  // os << "\t" << "id 42" << endl;
  os << "\t" << "label \"This is a gidd graph\"" << endl;

  nodeIds.empty();
  currentNodeId = 0;
}

void GmlGenerator::footer(std::ostream &os) {
  os << "]" << endl;
}

void GmlGenerator::nodeDefinition(std::ostream &os, std::shared_ptr<File> file, bool clusters) {
  /*
    node [
    	id 1
    	label "node 1"
    	thisIsASampleAttribute 42
    ]
   */
  currentNodeId++;
  nodeIds[file] = currentNodeId;

  os << "\tnode [" << endl;
  os << "\t\tid " << currentNodeId << endl;
  os << "\t\tlabel \"" << file->name<< "\"" << endl;
  os << "\t]" << endl;
}

void GmlGenerator::edgeDefinition(std::ostream &os, std::shared_ptr<File> src, std::shared_ptr<File> dst, bool clusters) {
  /*
    edge [
    	source 3
    	target 1
    	label "Edge from node 3 to node 1"
    ]
   */
  os << "\tedge [" << endl;
  os << "\t\tsource " << nodeIds[src] << endl;
  os << "\t\ttarget " << nodeIds[dst] << endl;
  os << "\t]" << endl;
}
