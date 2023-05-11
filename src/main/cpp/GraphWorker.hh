//
// Created by marco on 07/05/2023.
//

#ifndef SERVER_GRAPHWORKER_HH
#define SERVER_GRAPHWORKER_HH

#include "Graph/Edge.hh"
#include "Epoll/EpollInstance.hh"
#include "Epoll/MessageQueue/MessageQueue.hh"

#include <vector>
#include "Graph/Message.hh"

class GraphWorker {

		MessageQueue<Message> *inQueue;
		EpollInstance epollInstance;

public:

		void threadMain();

};


#endif //SERVER_GRAPHWORKER_HH
