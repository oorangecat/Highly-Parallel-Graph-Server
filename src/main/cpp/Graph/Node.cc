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

double Node::ddistance(uint32_t x, uint32_t y) {
	double dx = static_cast<double>(x)-static_cast<double>(this->x);
	double dy = static_cast<double>(y)-static_cast<double>(this->y);
	double dist=(dy*dy) + (dx*dx);

#if DATADEBUG==true
	//printf("DISTANCE (%d,%d) : (%d,%d) = %f\n", x, y, this->x, this->y, sqrt(dist));
#endif
	return sqrt(dist);
}

uint64_t Node::hash(){
/*
	if(this->chash == 0) {

		//split plane in cells, find closest grid point (up-left)
		double xi = std::floor(this->x / GRIDWIDTH) * GRIDWIDTH;
		double yi = std::floor(this->y / GRIDWIDTH) * GRIDWIDTH;

		double xres = 0;
		double yres = 0;
		double distres;
		double dist = this->ddistance(xi, yi);

		if ( (distres = this->ddistance(xi + GRIDWIDTH, yi)) < dist ) {	//(up,right)
			xres = xi + GRIDWIDTH;
			yres = yi;
			dist = distres;
		}

		if ( (distres = this->ddistance(xi + GRIDWIDTH, yi + GRIDWIDTH)) < dist) { //down,right
			xres = xi + GRIDWIDTH;
			yres = yi + GRIDWIDTH;
			dist = distres;
		}

		if ( (distres = this->ddistance(xi, yi + GRIDWIDTH) ) < dist) {					//down, left
			xres = xi;
			yres = yi + GRIDWIDTH;
			dist = distres;
		}

		this->chash = ((uint64_t)std::floor(xres) << 32) | (uint64_t) std::floor(yres);		//Identifies close point



	}*/
#if DATADEBUG==true
	//std::cout << "PTR:"<<this<<"|hash:"<<this->chash<<std::endl;
#endif
	return this->chash;

}

void Node::addEdge(Edge *w){

	uint64_t bhash = w->getB()->hash();

	auto res = this->edges->find(bhash);

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