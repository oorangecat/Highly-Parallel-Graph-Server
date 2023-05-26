//
// Created by marco on 07/05/2023.
//

#ifndef SERVER_NETWORKER_HH
#define SERVER_NETWORKER_HH

#include <vector>
#include <signal.h>

#include "Epoll/EpollInstance.hh"
#include "Epoll/EpollConnection.hh"
#include "Epoll/EpollIncoming.hh"
#include "Epoll/MessageQueue/MessageQueue.hh"
using namespace std;

class NetWorker {

	MessageQueue<int> *inQueue;
	EpollInstance epollInstance;

	Graph *graph;
public:
		NetWorker(MessageQueue<int> *inq, Graph *g);
		~NetWorker();

		void threadMain();


};


#endif //SERVER_NETWORKER_HH
