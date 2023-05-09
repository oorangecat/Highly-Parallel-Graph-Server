//
// Created by marco on 07/05/2023.
//

#include "NetWorker.hh"



NetWorker::NetWorker(int thread_fd, MessageQueue<int> *inq) {

	this->tfd = thread_fd;
	this->inQueue = inq;

}



void NetWorker::threadMain() {
	EpollInstance ep;
	EpollIncoming einc(this->tfd, &ep);

	ep.registerEpollEntry(einc);

	while (1) {
		ep.waitAndHandleEvents();
	}



}