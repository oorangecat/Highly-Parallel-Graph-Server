//
// Created by marco on 11/05/2023.
//

#include "EpollGraphMessage.hh"


EpollGraphMessage::EpollGraphMessage(EpollInstance *inst, MessageQueue<Message> *inq) {

	this->inst = inst;
	this->inqueue = inq;
	this->set_fd(inq->get_fd());
	this->set_events(EPOLLIN);


}



bool EpollGraphMessage::handleEvent(uint32_t events){

#if DEBUG == true
	printf("GraphThread %d received a new payload\n", this->get_fd());
	fflush(stdout);
#endif
	//cancel event
	uint64_t tmp;
	read(this->get_fd(), &tmp, sizeof(tmp));



	if ((events & EPOLLERR) || (events & EPOLLHUP) || !(events & EPOLLIN)) {
		return false;
	} else {

		Message rec = this->inqueue->pop();



	}
	return true;
};