//
// Created by marco on 07/05/2023.
//

#include "NetWorker.hh"
#include "Config.hh"



NetWorker::NetWorker(MessageQueue<int> *inq, MessageQueue<Message*> *graphq) {

	this->inQueue = inq;
	this->graphQueue = graphq;

}



void NetWorker::threadMain() {

#if DEBUG == true
	printf("Started new netthread %d\n", this->inQueue->get_fd());
#endif

	EpollInstance ep;
	EpollIncoming einc(&ep, this->inQueue, graphQueue);

	ep.registerEpollEntry(einc);

	while (1) {
		ep.waitAndHandleEvents();
	}



}

