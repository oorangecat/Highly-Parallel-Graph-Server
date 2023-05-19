//
// Created by marco on 11/05/2023.
//

#include "PointMap.hh"


PointMap::PointMap(){
	this->map = new std::unordered_map<uint64_t, Node* >();
	this->hashes = new std::unordered_map<uint32_t, std::vector<Node*>>();
	origin.set_p(0,0);
}



Node *PointMap::addPoint(Node *point){

	auto vec = map->find(this->hashNode(point));

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
	auto vec = map->find(this->hashNode(point));

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

/*

uint64_t PointMap::hashNode(Node *point) {
	uint64_t hashes[3][3];
	uint32_t dist[2][2];
	bool taken[3][3];
	double xi = std::floor(point->x_()/ GRIDWIDTH) * GRIDWIDTH;
	double yi = std::floor(point->y_() / GRIDWIDTH) * GRIDWIDTH;
	xi-=GRIDWIDTH; yi-=GRIDWIDTH;

	for(int i=0; i<3; i++) {
		for(int j=0; j<3; j++) {
			hashes[i][j] =((uint64_t)std::floor(xi+i*GRIDWIDTH) << 32) | (uint64_t) std::floor(yi+j*GRIDWIDTH);
		}
	}

	for(int i=0; i<3; i++) {
		for(int j=0; j<3; j++) {
			auto res = this->map->find(hashes[i][j]);
			if(res!= this->map->end()) {
				taken[i][j] = true;
				if (point->distance(res->second) <= MAXDIST){
					point->setHash(hashes[i][j]);
					return hashes[i][j];
				};
			} else {
				taken [i][j]=false;
			}
		}
	}

	uint32_t min_dist = -1;		//max uint
	int r=1;							//TODO fix segmentation, use error
	int c=1;

	for(int i = 1; i < 2; i++) {
		for (int j = 1; j < 2; j++) {
					dist[i-1][j-1] = point->distance(xi+i*GRIDWIDTH, yi+j*GRIDWIDTH);
		}
	}

	for(int i=0; i<2; i++){
		for(int j=0; j<2; j++){
			if(dist[i][j] < min_dist && !taken[i+1][j+1]){
				min_dist = dist[i][j];
				r = i; c = j;
			}
		}
	}
	point->setHash(hashes[r+1][c+1]);
	return hashes[r+1][c+1];

}*/


uint64_t PointMap::hashNode(Node *point){
	uint32_t distIdx = (std::floor(this->origin.ddistance(point->x_(), point->y_()) /GRIDWIDTH)-1) * GRIDWIDTH;		//starts from before ring
	//check before and after ring

	for(int i = 0; i <=2; i += 2) {

		auto res = this->hashes->find(distIdx+i*GRIDWIDTH);

		if (res != this->hashes->end()) {
			for (auto n: res->second) {

				if ( n->ddistance(point->x_(), point->y_()) <= MAXDIST ) {
					point->setHash(n->hash());
					return n->hash();
				}
			}
		}
	}

	distIdx += GRIDWIDTH;

	auto res = this->hashes->find(distIdx);

	if(res != this->hashes->end()){
		for(auto n: res->second){
			if ((n->ddistance(point->x_(), point->y_()) <= MAXDIST)) {
				point->setHash(n->hash());
				return n->hash();
			}
		}
		point->setHash(distIdx + res->second.size());
		res->second.push_back(point);
		return point->hash();
	} else {
		point->setHash(distIdx);
		std::vector<Node*> v;
		v.push_back(point);
		this->hashes->insert( std::make_pair(distIdx,v) );
		return distIdx;
	}
 return 0;

}