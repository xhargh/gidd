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

  clusterIds.empty();
  nodeIds.empty();
  currentNodeId = 0;
  currentClusterId = 0;
}

void GmlGenerator::footer(std::ostream &os, bool clusters) {
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
  /*
	node
	[
		id	0
		label	"gidd_utils.hpp"
		graphics
		[
			x	857.6746031746031
			y	226.7401153701776
			w	97.396484375
			h	30.0
			type	"rectangle"
			raisedBorder	0
			fill	"#FFCC00"
			outline	"#000000"
		]
		LabelGraphics
		[
			text	"gidd_utils.hpp"
			fontSize	12
			fontName	"Dialog"
			model	"null"
		]
	]
   */
  /* groups:
  	node
	[
		id	16
		label	"KalleAnka"
		graphics
		[
			fill	"#F5F5F5"
		]
		LabelGraphics
		[
			anchor	"t"
		]
		isGroup	1
	]

   then in a node, add "gid xx" where xx is the id of the group node
   */
  currentNodeId++;
  nodeIds[file] = currentNodeId;

  if (clusters) {
    if (clusterIds.find(file->path) == clusterIds.end()) {
      currentClusterId++;
      clusterIds[file->path] = currentClusterId;
    }
  }


}

void GmlGenerator::nodeDefinitionEnd(std::ostream &os, bool clusters) {
  for (auto& node : nodeIds) {
    os << "\tnode [" << endl;
    os << "\t\tid " << node.second << endl;
    os << "\t\tlabel \"" << node.first->name<< "\"" << endl;
    os << "\t\tgraphics [" << endl;
    os << "\t\t\tfill \"" << (node.first->isHeader()?"#ffcc00":"#00ccff") << "\"" << endl;
    os << "\t\t]" << endl; // graphics
    if (clusters) {
      os << "\t\tgid " << clusterIds[node.first->path] + currentNodeId << endl;
    }
    os << "\t]" << endl;
  }

  if (clusters) {
    for (auto& group : clusterIds) {
      os << "\tnode [" << endl;
      os << "\t\tid " << currentNodeId + group.second << endl;
      os << "\t\tlabel \"" << group.first << "\"" << endl;
      os << "\t\tgraphics [" << endl;
      os << "\t\t\tfill \"" << "#cccccc" << "\"" << endl;
      os << "\t\t]" << endl; // graphics
      os << "\t\tLabelGraphics" << endl;
      os << "\t\t[" << endl;
      os << "\t\t\tanchor \"t\"" << endl;
      os << "\t\t]" << endl; // LabelGraphics
      os << "\t\tisGroup 1" << endl;
      os << "\t]" << endl;
    }
  }
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
