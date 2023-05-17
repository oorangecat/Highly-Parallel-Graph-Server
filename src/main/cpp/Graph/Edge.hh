//
// Created by marco on 10/05/2023.
//

#ifndef SERVER_EDGE_HH
#define SERVER_EDGE_HH


#include <vector>
#include <cstdint>

class Node;		//circular dependency



class Edge {
		Node *a, *b;
		uint32_t dist;
		uint32_t nWalks = 1;


public:
		Edge(Node *a, Node *b, uint32_t dist);
		Edge(int32_t xa, int32_t ya, int32_t xb, int32_t yb, uint32_t dist);

		void addWalk(uint32_t newdist);
		void setDist(uint32_t dist);
		uint32_t getDist() { return this->dist;};

		Node *getA(){ return this->a; };
		Node *getB(){ return this->b; };
		void setA(Node* n){ this->a=n; };
		void setB(Node* n){ this->b=n; };

		//For usage in PriorityQueue
		bool operator<(const Edge other) const {
			return this->dist > other.dist;
		}

};


#endif //SERVER_EDGE_HH
