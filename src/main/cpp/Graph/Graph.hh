//
// Created by marco on 07/05/2023.
//

#ifndef SERVER_GRAPH_HH
#define SERVER_GRAPH_HH

#include "Edge.hh"
#include "Node.hh"
#include "PointMap.hh"
#include "../Config.hh"


//TODO implement graph parallelism via RCU in Graph class
class Graph {
		PointMap pointmap;

public:
		Graph() = default;
		void addWalk(Edge newWalk);
		void addWalkVector(std::vector<Edge> walks);
		void addLocation(Node *nnode);
		uint32_t closestDistance(Node *loc);

		Node* closestPoint(Node *p);

		uint64_t shortestToOne(Node *source, Node *dest);
		uint64_t shortestToAll(Node *source);


};



#endif //SERVER_GRAPH_HH
