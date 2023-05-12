//
// Created by marco on 07/05/2023.
//

#ifndef SERVER_GRAPHWORKER_HH
#define SERVER_GRAPHWORKER_HH

#include "Graph/Edge.hh"
#include "Epoll/EpollInstance.hh"
#include "Epoll/MessageQueue/MessageQueue.hh"
#include "Epoll/EpollGraphMessage.hh"

#include <vector>
#include "Graph/Message.hh"

class GraphWorker {

		MessageQueue<Message*> *inQueue;
		EpollInstance epollInstance;

public:
		GraphWorker(MessageQueue<Message*> *inq) { this->inQueue = inq;};

		void threadMain();

};


#endif //SERVER_GRAPHWORKER_HH
