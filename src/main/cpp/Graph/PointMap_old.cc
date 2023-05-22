//
// Created by marco on 11/05/2023.
//

#include "PointMap.hh"


PointMap::PointMap(){

	this->map = new std::unordered_map<uint64_t, Node* >();

#if GRIDMAP == true
	this->grid = new std::unordered_map<uint64_t, std::vector<Node*>>();
#endif
	origin.set_p(0,0);
}

Node* PointMap::queryRange(Node* center, double range) {

	uint cellX = std::floor(center->x_() / GRIDWIDTH);
	uint cellY = std::floor(center->y_() / GRIDWIDTH);

	uint startX = std::max<int>(0, cellX - 1);
	uint startY = std::max<int>(0, cellY - 1);

	uint endX = cellX + 1;
	uint endY = cellY + 1;
	uint64_t cellId;
	for (uint32_t x = startX; x <= endX; x++) {
		for (uint32_t y = startY; y <= endY; y++) {
			cellId = x + (y << 32);

			auto cellIt = this->grid->find(cellId);

			if (cellIt != this->grid->end()) {

				const std::vector<Node *> pointsInCell = cellIt->second;

#if DATADEBUG == true
				std::cout<<"vec dim: "<<pointsInCell.size()<<" cellId: "<<cellId<<std::endl;
#endif
				for (Node *point: pointsInCell) {

					double distance = std::hypot(point->x_() - center->x_(), point->y_() - center->y_());
					if (distance <= range) {
						return point;
					}

				}
			}
		}
	}
	return nullptr;
}


Node *PointMap::addPoint(Node *point) {
#if GRIDMAP == false
	auto vec = map->find(this->hashNode(point));

	if(vec != map->end()) {
		return vec->second;
	}
	//One entry for each Node cluster
	this->map->insert(std::make_pair(point->hash(), point));

	this->ssize++;
	return point;
#elif GRIDMAP == true
	Node* res = queryRange(point, MAXDIST);

	if(res != nullptr) {
		delete(point);
		return res;
	} else {

#if DEBUG==true
		if(!(this->lasthash % 5000 ))
			std::cout<<"lasthash: "<<this->lasthash<<std::endl;
		if(point->x_()<0 || point->y_()<0) {std::cout<<"\n\n\n\n\nLESS THAN 0 COORD ("<<point->x_()<<","<<point->y_()<<")\n\n\n\n"<<std::endl;}
#endif

		point->setHash(++(this->lasthash));
		this->map->insert(std::make_pair(point->hash(), point));
		this->ssize++;

		uint32_t cellX = std::floor(point->x_() / GRIDWIDTH);
		uint32_t cellY = std::floor(point->y_() / GRIDWIDTH);

		uint64_t cellId = cellX + (cellY << 32);
		auto v = this->grid->find(cellId);
		if (v != this->grid->end()) {
			v->second.push_back(point);
		} else {
			std::vector<Node *> v;
			v.push_back(point);
			this->grid->insert(std::make_pair(cellId, v));
		}
		return point;
	}
#endif
}

void PointMap::addPoint(int32_t x, int32_t y){
	Node newnode(x,y);
	this->addPoint(&newnode);
}

Node* PointMap::closestPoint(Node *point){
#if GRIDMAP == true
	auto vec = queryRange(point,MAXDIST);

	if(vec != nullptr) {
		return vec;
	}	else {
		return point;
	}
#endif
}

Node *PointMap::closestPoint(int32_t x, int32_t y){
	Node newnode(x,y);
	return this->closestPoint(&newnode);
}

/*
uint32_t PointMap::closestDistance(Node *point){
	Node *p = this->closestPoint(point);

	return point->distance(p);
}
 */

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

#if STUPIDHASH == false && GRIDMAP == false
uint64_t PointMap::hashNode(Node *point){

	uint64_t distIdx = (std::floor(this->origin.ddistance(point->x_(), point->y_()) /GRIDWIDTH)) * GRIDWIDTH;		//starts from before ring
	//check before and after ring

	for(int i = -GRIDWIDTH; i <=GRIDWIDTH; i += 2*GRIDWIDTH) {
		auto res = this->hashes->find(distIdx+i);

		if (res != this->hashes->end()) {
			for (auto n: res->second) {

				if ( n->ddistance(point->x_(), point->y_()) <= MAXDIST ) {
					point->setHash(n->hash());
					return n->hash();
				}
			}
		}
	}

	//distIdx += GRIDWIDTH;

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
#elif STUPIDHASH==true
uint64_t PointMap::hashNode(Node *point) {
	for(auto n : *(this->hashes)){
		if(n.second->distance(point->x_(), point->y_()) <= MAXDIST){
			point->setHash(n.second->hash());
			return n.second->hash();
		}
	}

#if DEBUG==true				//log every 1000 added
	if(this->lasthash % 1000 == 0)
		std::cout<<"LastHash:"<<this->lasthash<<std::endl;
#endif
	uint64_t lasth = this->lasthash;
	point->setHash(lasth);
	this->lasthash++;
	this->hashes->insert(std::make_pair(lasth, point));
	return lasth;
}
#endif

