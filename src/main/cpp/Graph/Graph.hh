//
// Created by marco on 07/05/2023.
//

#ifndef SERVER_GRAPH_HH
#define SERVER_GRAPH_HH
#include <unordered_map>

#include "Edge.hh"
#include "Node.hh"
#include "PointMap.hh"
#include "../Config.hh"

#if USERCU == true
#include <urcu/urcu-qsbr.h>
#include <mutex>
#endif

#if USERCU == false
#include <pthread.h>
#endif


#include <unordered_map>
#include <unordered_set>
#include <queue>
/*
 * Graph data structure
 * Read protected by RCU
 * Concurrent write avoided by mutex
 */






class Graph {
		PointMap *pointmap;
		unordered_map<uint64_t,uint64_t> *cache;
		pthread_rwlock_t rwlock;

		uint64_t sumPath(std::unordered_map<Node*, Node*> *parent, Node* source, Node* dest);
		uint64_t sumMap(std::unordered_map<Node*,uint64_t> *map);

public:
		Graph() {
			this->pointmap = new PointMap();
			this->cache = new unordered_map<uint64_t,uint64_t>;
			pthread_rwlock_init(&rwlock, NULL);
		};
		void addWalkUnsync(Edge newWalk);
		void addWalkVector(std::vector<Edge*> walks);
		void addLocation(Node *nnode);
		uint32_t closestDistance(Node *loc);


		Node* closestPoint(Node *p);
		Node* closestPoint(uint32_t x, uint32_t y);


			Node* addPoint(Node *p);

		uint64_t shortestToOne(Node *source, Node *dest);
		uint64_t shortestToAll(Node *source);







		};



#endif //SERVER_GRAPH_HH
