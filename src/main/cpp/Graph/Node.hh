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
		int64_t chash = -1;

		//<hash_destination, edge>
		std::unordered_map<int64_t, Edge*> edges;

public:

		Node() = default;
		Node(int32_t x, int32_t y) { this->x=x; this->y=y; };

		void set_p(int32_t x, int32_t y);

		int32_t distance(int32_t x, int32_t y);
		int32_t distance(Node *other);
		void addEdge(Edge *w);

		int64_t hash();


};


#endif //SERVER_NODE_HH
