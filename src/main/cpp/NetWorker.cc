//
// Created by marco on 07/05/2023.
//

#include "NetWorker.hh"
#include "Config.hh"



NetWorker::NetWorker(MessageQueue<int> *inq, Graph *g) {

	this->inQueue = inq;
	this->graph = g;

}



void NetWorker::threadMain() {

#if DEBUG == true
	printf("Started new netthread %d\n", this->inQueue->get_fd());
#endif

	EpollInstance ep;
	EpollIncoming einc(&ep, this->inQueue, this->graph);

	ep.registerEpollEntry(einc);

	while (1) {
		ep.waitAndHandleEvents();
	}



}

