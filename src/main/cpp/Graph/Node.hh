//
// Created by marco on 10/05/2023.
//

#ifndef SERVER_NODE_HH
#define SERVER_NODE_HH

#include "Edge.hh"

#include "../Config.hh"

#include <unordered_map>

// Used to hold details of a point
struct Point {
		int x;
		int y;
		Point(int _x, int _y)
		{
			x = _x;
			y = _y;
		}
		Point()
		{
			x = 0;
			y = 0;
		}
};

class Node {
		int32_t x,y;
		uint64_t chash = 0;
		uint64_t dist;
		//<hash_destination, edge>
		std::unordered_map<Node*, Edge*> *edges;

public:
		Point pos;			//To be used with quadtree

		Node() { this->edges = new std::unordered_map<Node*, Edge*>;	};
		Node(int32_t x, int32_t y) {
			this->x=x; this->y=y;  this->edges = new std::unordered_map<Node*, Edge*>; this->pos = Point(x,y);} ;

		void set_p(int32_t x, int32_t y);
		int32_t x_(){return x;};
		int32_t y_(){return y;};
		void setHash(uint64_t h){ this->chash=h;};


		uint32_t distance(uint32_t x, uint32_t y);
		int32_t distance(Node *other);
		double ddistance(uint32_t x, uint32_t y);


		int32_t edgeDist(Node *other);

		void addEdge(Edge *w);
		std::unordered_map<Node*, Edge*> *getEdges( ) { return this->edges; };
		uint64_t hash();

		 uint64_t getDistance(){ return this->dist;};
		 void setDistance(uint64_t d) { this->dist = d; };

		bool operator>(Node* other) const {
			return this->dist > other->getDistance();
		}

};




#endif //SERVER_NODE_HH
