//
// Created by marco on 07/05/2023.
//

#ifndef SERVER_GRAPHWORKER_HH
#define SERVER_GRAPHWORKER_HH

#include "Graph/Edge.hh"
#include "Epoll/EpollInstance.hh"
#include "Epoll/MessageQueue/MessageQueue.hh"
#include "Epoll/EpollGraphMessage.hh"
#include "Graph/Message.hh"
#include "Graph/Graph.hh"		//includes URCU lib

#include <vector>



class GraphWorker {

		MessageQueue<Message*> *inQueue;
		EpollInstance epollInstance;
		Graph *graph;

public:
		GraphWorker(MessageQueue<Message*> *inq, Graph *g) {
			this->inQueue = inq;
			this->graph = g;
		};

		void threadMain();

};


#endif //SERVER_GRAPHWORKER_HH
