//
// Created by marco on 07/05/2023.
//

#include "NetWorker.hh"
#include "Config.hh"



NetWorker::NetWorker(MessageQueue<int> *inq) {

	this->inQueue = inq;

}



void NetWorker::threadMain() {

#if DEBUG == true
	printf("Started new thread %d\n", this->inQueue->get_fd());
#endif

	EpollInstance ep;
	EpollIncoming einc(&ep, this->inQueue);

	ep.registerEpollEntry(einc);

	while (1) {
		ep.waitAndHandleEvents();
	}



}

/*
 * TODO Implement routes for messages
 * TODO Implement GRAPH data structure and walk-location handling
 * TODO Implement all GRAPH logic into graph handler
 */