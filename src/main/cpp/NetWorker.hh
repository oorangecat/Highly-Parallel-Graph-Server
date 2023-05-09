//
// Created by marco on 07/05/2023.
//

#ifndef SERVER_NETWORKER_HH
#define SERVER_NETWORKER_HH

#include <vector>
#include "Epoll/EpollInstance.hh"
#include "EpollConnection.hh"
#include "Epoll/EpollIncoming.hh"
#include "MessageQueue/MessageQueue.hh"

using namespace std;

class NetWorker {

	MessageQueue<int> *inQueue;
	EpollInstance epollInstance;

	int tfd;

public:
		NetWorker(int thread_fd, MessageQueue<int> *inq);
		~NetWorker();


		void threadMain();




};


#endif //SERVER_NETWORKER_HH
