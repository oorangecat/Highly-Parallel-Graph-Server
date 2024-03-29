//
// Created by marco on 10/05/2023.
//

#include "Edge.hh"
#include "Node.hh"


Edge::Edge(Node *a, Node *b, uint32_t dist) {
	this->a = a;
	this->b = b;
	this->dist = dist;
}

Edge::Edge(int32_t xa, int32_t ya, int32_t xb, int32_t yb, uint32_t dist){
	Node s(xa, ya);
	Node e(xb, yb);

	this->a = &s;
	this->b = &e;
	this->dist = dist;

}

void Edge::addWalk(uint32_t newdist){
	//std::cout<<"Prev: "<<this->dist;
	this->dist = (((uint64_t)this->dist * (uint64_t)this->nWalks) + newdist) / (uint64_t)(this->nWalks+1);
	this->nWalks++;
//	std::cout<<" | New: "<<this->dist<<std::endl;

}

void Edge::setDist(uint32_t dist){
	this->dist = dist;
}