//
// Created by marco on 11/05/2023.
//

#include "PointMap.hh"
#include <limits>



PointMap::PointMap(){
	this->grid = new unordered_map<int64_t,vector<Node*>*>; this->zero=new Node(0,0);
};
PointMap::~PointMap(){delete(this->grid); delete(this->zero);};

Node *PointMap::addPoint(Node *point){

	/*
	std::vector<Node*> res = this->tree.getPointsCloserThanMaxDist(Point(point->x_(),point->y_()));
	if(res.empty()){
		this->tree.insert(point);
		this->ssize++;
		return point;
	} else {
		delete(point);
		return res.front();
	}*/

	Node* closest = closestPoint(point->x_(), point->y_());

	if(closest == nullptr){
		//uint64_t ringid = std::round(std::hypot((uint32_t)point->x_(), (uint32_t)point->y_())) / (GRIDWIDTH);
		int64_t ringid=(point->x_()+point->y_())>>10;
#if DEBUG == true
	//	std::cout<<"Adding ("<<point->x_()<<","<<point->y_()<<") ringId "<<ringid<<"\n"<<std::endl;
#endif

		auto res = this->grid->find(ringid);
		//Node *n = new Node(point->x_(), point->y_());

		if(res==this->grid->end()) {
			vector<Node *> *v = new vector<Node *>();
			v->push_back(point);
			this->grid->insert(std::make_pair(ringid, v));
		} else {
			res->second->push_back(point);
		}
			this->ssize++;

		return point;

	} else {
		return closest;
	}

}

uint64_t PointMap::totEdges(){
	uint64_t tot=0;
	for(auto p : *this->grid){
		for(auto n : *p.second){
			tot+=n->getEdges()->size();
		}
	}
	return tot;
}


Node* PointMap::closestPoint(int32_t x, int32_t y){

//uint64_t ringid = std::round(std::hypot((uint32_t)x, (uint32_t)y) / GRIDWIDTH);
	int64_t ringid=(x+y)>>10;
	for(int i = -20; i <= 20; i++) {
		auto res = this->grid->find(ringid + i);

		if(res !=  this->grid->end()){
			for (auto n: *res->second) {
				if(n->x_() == x && n->y_() ==y)
					return n;
				if (std::hypot(n->x_()-x, n->y_()-y) <= MAXDIST) {
					return n;
				}
			}
		}

		//	}
	}
	return nullptr;
}


