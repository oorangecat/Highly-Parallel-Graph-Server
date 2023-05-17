//
// Created by marco on 11/05/2023.
//

#include "PointMap.hh"


PointMap::PointMap(){
	this->map = new std::unordered_map<int64_t, Node* >();
}



Node *PointMap::addPoint(Node *point){
	auto vec = map->find( point->hash() );

	if(vec != map->end()){
		return vec->second;
	}
	//One entry for each Node cluster
	this->map->insert(std::make_pair(point->hash(), point));

	this->ssize++;
	return point;
}

void PointMap::addPoint(int32_t x, int32_t y){
	Node newnode(x,y);
	this->addPoint(&newnode);
}

Node* PointMap::closestPoint(Node *point){
	auto vec = map->find(point->hash());

	if(vec != map->end()) {
		return vec->second;
	}	else {
		return point;
	}

}

Node *PointMap::closestPoint(int32_t x, int32_t y){
	Node newnode(x,y);
	return this->closestPoint(&newnode);
}


uint32_t PointMap::closestDistance(Node *point){
	Node *p = this->closestPoint(point);

	return point->distance(p);
}