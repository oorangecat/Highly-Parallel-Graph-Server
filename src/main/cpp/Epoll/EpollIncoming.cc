//
// Created by marco on 07/05/2023.
//
#include "EpollConnection.hh"
#include "EpollIncoming.hh"
#include "Config.hh"
#include <sys/epoll.h>


EpollIncoming::EpollIncoming(EpollInstance *inst, MessageQueue<int> *inq, MessageQueue<Message*> *outq) {

	this->inst = inst;
	this->inqueue = inq;
	this->outqueue = outq;
	this->set_fd(inq->get_fd());
	this->set_events(EPOLLIN);


}

EpollIncoming::~EpollIncoming(){

};

bool EpollIncoming::handleEvent(uint32_t events){

#if DEBUG == true
	printf("NetThread %d received a new connection\n", this->get_fd());
	fflush(stdout);
#endif
	uint64_t tmp;
	read(this->get_fd(), &tmp, sizeof(tmp));
	int *rec;

	if ((events & EPOLLERR) || (events & EPOLLHUP) || !(events & EPOLLIN)) {
		return false;
	} else {
		//ssize_t count = read(this->t_fd, &rec, sizeof(rec));	//PIPE approach discarded

		rec = this->inqueue->pop();		//TODO check performance, includes blocking op for mutex

		if(rec!=nullptr) {
			conn_t *newconn = new conn_t;
			newconn->cfd = *rec;

			EpollConnection *newepconn = new EpollConnection(newconn, this->outqueue);

			this->inst->registerEpollEntry(*newepconn);      //Registers new connection in the thread poll
		}
	}
	return true;
};