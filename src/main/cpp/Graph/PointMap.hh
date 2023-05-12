//
// Created by marco on 11/05/2023.
//

#ifndef SERVER_POINTMAP_HH
#define SERVER_POINTMAP_HH
#include <vector>
#include <unordered_map>

#include "Node.hh"

class PointMap {

		std::unordered_map<int64_t, Node* > *map;
		int ssize=0;
public:
		PointMap();
		~PointMap(){ delete (map);} ;
		PointMap(std::unordered_map<int64_t, Node* > *smap) { this->map = smap;};
		Node *addPoint(Node *point);

		void addPoint(int32_t x, int32_t y);
		//void swapMap(std::unordered_map<int64_t, Node*> *newmap){delete(this->map); this->map=newmap;	};
		std::unordered_map<int64_t, Node* >* getMap(){ return this->map;};
		Node* closestPoint(Node *point);
		Node* closestPoint(int32_t x, int32_t y);

		uint32_t closestDistance(Node *point);
		int size() { return this->ssize; };
};


#endif //SERVER_POINTMAP_HH
