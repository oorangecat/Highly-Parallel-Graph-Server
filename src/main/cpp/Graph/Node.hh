//
// Created by marco on 10/05/2023.
//

#ifndef SERVER_NODE_HH
#define SERVER_NODE_HH

#include "Edge.hh"

#include "../Config.hh"

#include <unordered_map>


class Node {
		int32_t x,y;
		uint64_t chash = 0;

		//<hash_destination, edge>
		std::unordered_map<uint64_t, Edge*> *edges;

public:

		Node() { this->edges = new std::unordered_map<uint64_t, Edge*>;};
		Node(int32_t x, int32_t y) { this->x=x; this->y=y;  this->edges = new std::unordered_map<uint64_t, Edge*>;};

		void set_p(int32_t x, int32_t y);
		int32_t x_(){return x;};
		int32_t y_(){return y;};
		void setHash(uint64_t h){ this->chash=h;};
		//TODO [mm] precision, might be improved with fp result (worth it?)
		uint32_t distance(uint32_t x, uint32_t y);
		int32_t distance(Node *other);
		double ddistance(uint32_t x, uint32_t y);


			int32_t edgeDist(Node *other);

		void addEdge(Edge *w);
		std::unordered_map<uint64_t, Edge*> *getEdges( ) { return this->edges; };

		uint64_t hash();


};




#endif //SERVER_NODE_HH
