//
// Created by marco on 07/05/2023.
//

#include "GraphWorker.hh"


void GraphWorker::threadMain() {

#if DEBUG == true
	printf("Started new graphthread %d\n", this->inQueue->get_fd());
#endif

	EpollInstance ep;
	EpollGraphMessage einc(&ep, this->inQueue);

	ep.registerEpollEntry(einc);

	while (1) {
		ep.waitAndHandleEvents();
	}



}