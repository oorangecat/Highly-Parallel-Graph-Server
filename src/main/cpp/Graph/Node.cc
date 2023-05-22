//
// Created by marco on 10/05/2023.
//

#include "Node.hh"
#include	<math.h>




void Node::set_p(int32_t x, int32_t y){
	this->x=x; this->y=y;
}

//A* heuristic for 
int32_t Node::distance(Node *other) {
	return distance(other->x, other->y);
}


uint32_t Node::distance(uint32_t x, uint32_t y){
	uint64_t dx = x-this->x;
	uint64_t dy = y-this->y;
	double dist = (dy*dy) + (dx*dx);
	return (uint32_t) std::floor( sqrt( dist ) );
}

double Node::ddistance(uint32_t x, uint32_t y) {
	double dx = static_cast<double>(x)-static_cast<double>(this->x);
	double dy = static_cast<double>(y)-static_cast<double>(this->y);
	double dist=(dy*dy) + (dx*dx);

#if DISTDEBUG==true
	printf("DISTANCE (%d,%d) : (%d,%d) = %f\n", x, y, this->x, this->y, sqrt(dist));
#endif
	return sqrt(dist);
}

uint64_t Node::hash(){	//To be used only for print

	return (this->x+this->y);

}

void Node::addEdge(Edge *w){


	auto res = this->edges->find(w->getB());

	if(res != this->edges->end()){
#if DATADEBUG == true
		printf("Edge %ld -> %ld already present\n", this->hash(), w->getB()->hash());
		fflush(stdout);
#endif
		res->second->addWalk(w->getDist());
	} else {
#if DATADEBUG == true
		printf("Edge %ld -> %ld added, dist: %d\n", this->hash(), w->getB()->hash(), w->getDist());
		fflush(stdout);
#endif
		this->edges->insert( std::make_pair(w->getB(),w) );
	}
}

int32_t Node::edgeDist(Node *other){
	auto res = this->edges->find(other);
	if(res != this->edges->end())
		return res->second->getDist();
	else
		return -1;
}