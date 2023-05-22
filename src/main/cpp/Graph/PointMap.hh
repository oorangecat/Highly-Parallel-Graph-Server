//
// Created by marco on 11/05/2023.
//

#ifndef SERVER_POINTMAP_HH
#define SERVER_POINTMAP_HH
#include <vector>
#include <unordered_map>
#include <math.h>

#include "QuadTree.hh"
#include "Node.hh"

class PointMap {
	//QuadTree tree;
		unordered_map<int64_t,vector<Node*>*> *grid;
		Node *zero;
		int ssize = 0;			//number of nodes
public:
		PointMap();
		~PointMap();

		Node *addPoint(Node *point);

		Node* closestPoint(int32_t x, int32_t y);

		int size() { return this->ssize; };
		uint64_t totEdges();
};


#endif //SERVER_POINTMAP_HH
