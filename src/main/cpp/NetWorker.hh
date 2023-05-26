//
// Created by marco on 07/05/2023.
//

#ifndef SERVER_NETWORKER_HH
#define SERVER_NETWORKER_HH

#include <vector>
#include "Epoll/EpollInstance.hh"
#include "Epoll/EpollConnection.hh"
#include "Epoll/EpollIncoming.hh"
#include "Epoll/EpollResult.hh"
#include "Epoll/MessageQueue/MessageQueue.hh"
#include "Graph/Message.hh"
#include "Graph/Result.hh"
using namespace std;

class NetWorker {

	MessageQueue<int> *inQueue;
	EpollInstance epollInstance;

	MessageQueue<Message*> *graphQueue;
	MessageQueue<Result*> *retQueue;
	Graph *graph;
public:
		NetWorker(MessageQueue<int> *inq, MessageQueue<Message*> *graphQueue, Graph *g);
		~NetWorker();

		void threadMain();


};


#endif //SERVER_NETWORKER_HH
