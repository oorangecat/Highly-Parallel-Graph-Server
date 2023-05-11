//
// Created by marco on 07/05/2023.
//

#include "Graph.hh"


void Graph::addWalk(Edge newWalk){
	Node* res =	this->pointmap.addPoint(newWalk.getA());
	this->addLocation(newWalk.getB());
	res->addEdge(&newWalk);
}


void Graph::addWalkVector(std::vector<Edge> walks){
	for(auto el : walks) {
		this->addWalk(el);
	}
}


void Graph::addLocation(Node *nnode){
	this->pointmap.addPoint(nnode);
}


uint32_t Graph::closestDistance(Node* loc) {
	this->pointmap.closestDistance(loc);
}

Node* Graph::closestPoint(Node *p){
	return this->pointmap.closestPoint(p);
}