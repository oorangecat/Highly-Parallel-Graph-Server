//
// Created by marco on 07/05/2023.
//

#ifndef SERVER_GRAPH_HH
#define SERVER_GRAPH_HH
#

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






//TODO implement graph parallelism via RCU in Graph class
class Graph {
		PointMap *pointmap;
#if USERCU == true
		std::mutex writelock;
#elif USERCU == false
		pthread_rwlock_t rwlock;
#endif

		uint64_t sumPath(std::unordered_map<Node*, Node*> *parent, Node* source, Node* dest);
		uint64_t sumMap(std::unordered_map<Node*,uint64_t> *map);

public:
		Graph() {
			this->pointmap = new PointMap();
#if USERCU==false
			pthread_rwlock_init(&rwlock, NULL);
#endif
		};
		void addWalkUnsync(Edge newWalk);
		void addWalkVector(std::vector<Edge*> walks);
		void addLocation(Node *nnode);
		uint32_t closestDistance(Node *loc);


		Node* closestPoint(Node *p);

		Node* addPoint(Node *p);

		uint64_t shortestToOne(Node *source, Node *dest);
		uint64_t shortestToAll(Node *source);







		};



#endif //SERVER_GRAPH_HH
