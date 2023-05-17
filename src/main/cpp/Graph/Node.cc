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
	double dx = x-this->x;
	double dy = y-this->y;
	double dist = (dy*dy) + (dx*dx);
	return (uint32_t) std::floor( sqrt( dist ) );
}


uint64_t Node::hash(){

	if(this->chash == 0) {

		//split plane in cells, find closest grid point (up-left)
		uint32_t xi = std::floor(this->x / GRIDWIDTH) * GRIDWIDTH;
		uint32_t yi = std::floor(this->y / GRIDWIDTH) * GRIDWIDTH;

		uint32_t xres = 0;
		uint32_t yres = 0;
		uint32_t distres;
		uint32_t dist = this->distance(xi, yi);

		if ( (distres = this->distance(xi + GRIDWIDTH, yi)) < dist ) {	//(up,right)
			xres = xi + GRIDWIDTH;
			yres = yi;
			dist = distres;
		}

		if ( (distres = this->distance(xi + GRIDWIDTH, yi + GRIDWIDTH)) < dist) { //down,right
			xres = xi + GRIDWIDTH;
			yres = yi + GRIDWIDTH;
			dist = distres;
		}

		if ( (distres = this->distance(xi, yi + GRIDWIDTH) ) < dist) {					//down, left
			xres = xi;
			yres = yi + GRIDWIDTH;
			dist = distres;
		}

		this->chash = ((uint64_t)xres << 32) | yres;		//Identifies close point



	}
#if DEBUG==true
	//std::cout << "PTR:"<<this<<"|hash:"<<this->chash<<std::endl;
#endif
	return this->chash;

}

void Node::addEdge(Edge *w){

	uint64_t bhash = w->getB()->hash();

	auto res = this->edges->find(bhash);

	if(res != this->edges->end()){
#if DEBUG == true
		printf("Edge %ld -> %ld already present\n", this->hash(), w->getB()->hash());
		fflush(stdout);
#endif
		res->second->addWalk(w->getDist());
	} else {
#if DEBUG == true
		printf("Edge %ld -> %ld added, dist: %d\n", this->hash(), w->getB()->hash(), w->getDist());
		fflush(stdout);
#endif
		this->edges->insert( std::make_pair(bhash,w) );
	}
}

int32_t Node::edgeDist(Node *other){
	auto res = this->edges->find(other->hash());
	if(res != this->edges->end())
		return res->second->getDist();
	else
		return -1;
}