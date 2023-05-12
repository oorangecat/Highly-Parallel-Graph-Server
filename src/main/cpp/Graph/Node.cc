//
// Created by marco on 10/05/2023.
//

#include "Node.hh"
#include	<math.h>




void Node::set_p(int32_t x, int32_t y){
	this->x=x; this->y=y;
}


int32_t Node::distance(Node *other) {
	return distance(other->x, other->y);
}


int32_t Node::distance(int32_t x, int32_t y){
	double dx = x-this->x;
	double dy = y-this->y;
	double dist = dy*dy+dx*dx;
	return (int32_t)std::floor(sqrt(dist));
}


int64_t Node::hash(){

	if(this->chash == -1) {
		//split plane in cells, find closest grid point (up-left)
		int32_t xi = std::floor(this->x / GRIDWIDTH);
		int32_t yi = std::floor(this->y / GRIDWIDTH);

		int32_t xres, yres, distres;
		int32_t dist = this->distance(xi, yi);

		if ((distres = this->distance(xi + GRIDWIDTH, yi)) < dist) {	//(up,right)
			xres = xi + GRIDWIDTH;
			yres = yi;
		}

		if ((distres = this->distance(xi + GRIDWIDTH, yi + GRIDWIDTH)) < dist) { //down,right
			xres = xi + GRIDWIDTH;
			yres = yi + GRIDWIDTH;
			dist = distres;
		}

		if ((distres = this->distance(xi, yi + GRIDWIDTH)) < dist) {					//down, left
			xres = xi;
			yres = yi + GRIDWIDTH;
			dist = distres;
		}

		this->chash = ((int64_t)xres << 32) | yres;		//Identifies close point

	}
	return this->chash;

}

void Node::addEdge(Edge *w){

	int64_t bhash=w->getB()->hash();

	auto res = this->edges.find(bhash);

	if(res != this->edges.end()){
		res->second->addWalk(w->getDist());
	} else {
		this->edges.insert(std::make_pair(bhash,w));
	}
}