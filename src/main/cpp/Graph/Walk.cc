//
// Created by marco on 10/05/2023.
//

#include "Walk.hh"


Walk::Walk(Location a, Location b, uint32_t dist) {
	this->a = a;
	this->b = b;
	this->dist = dist;
}

Walk::Walk(int32_t xa, int32_t ya, int32_t xb, int32_t yb, uint32_t dist){
	Location s(xa, ya);
	Location e(xb, yb);

	this->a = s;
	this->b = e;
	this->dist = dist;

}

void Walk::addWalk(uint32_t newdist){
	this->dist = ((this->dist * this->nWalks) + newdist) / ++this->nWalks;;
}
void Walk::setDist(uint32_t dist){
	this->dist = dist;
}