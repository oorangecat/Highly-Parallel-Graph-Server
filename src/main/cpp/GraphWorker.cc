//
// Created by marco on 07/05/2023.
//

#include "GraphWorker.hh"


void GraphWorker::threadMain() {

#if DEBUG == true
	printf("Started new graphthread %d\n", this->inQueue->get_fd());
#endif

	urcu_qsbr_register_thread();		//Register thread for reading
	urcu_qsbr_quiescent_state();		//Declare current quiescent state
	EpollInstance ep;
	EpollGraphMessage einc(&ep, this->inQueue, this->graph);

	ep.registerEpollEntry(einc);

	while (1) {
		ep.waitAndHandleEvents();
	}

	urcu_qsbr_unregister_thread();

}