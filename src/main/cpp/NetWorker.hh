//
// Created by marco on 07/05/2023.
//

#ifndef SERVER_NETWORKER_HH
#define SERVER_NETWORKER_HH

#include <vector>
#include "Epoll/EpollInstance.hh"
#include "Epoll/EpollConnection.hh"
#include "Epoll/EpollIncoming.hh"
#include "Epoll/MessageQueue/MessageQueue.hh"

using namespace std;

class NetWorker {

	MessageQueue<int> *inQueue;
	EpollInstance epollInstance;


public:
		NetWorker(MessageQueue<int> *inq);
		~NetWorker();


		void threadMain();




};


#endif //SERVER_NETWORKER_HH
