//
// Created by marco on 11/05/2023.
//

#include "EpollGraphMessage.hh"
//TODO implement graph parallelism via RCU in Worker


EpollGraphMessage::EpollGraphMessage(EpollInstance *inst, MessageQueue<Message*> *inq) {

	this->inst = inst;
	this->inqueue = inq;
	this->set_fd(inq->get_fd());
	this->set_events(EPOLLIN);

}



bool EpollGraphMessage::handleEvent(uint32_t events){

#if DEBUG == true
	printf("GraphThread %d received a new payload\n", this->id);
	fflush(stdout);
#endif

	//cancel event
	uint64_t tmp;
	//different threads clearing the message could cause some messages to be ignored?
	read(this->get_fd(), &tmp, sizeof(tmp));

	if ((events & EPOLLERR) || (events & EPOLLHUP) || !(events & EPOLLIN)) {
		return false;
	} else {

		Message **tmp = this->inqueue->pop();

		if(tmp!=nullptr){			//if thread manages to get the message first
			Message *rec = *tmp;
#if DEBUG == true
			printf(" Message type: %c\n", rec->isToMany() ? 'm':'s');
			fflush(stdout);
#endif

			delete(*tmp);

		}



	}
	return true;
};

EpollGraphMessage::~EpollGraphMessage(){}
