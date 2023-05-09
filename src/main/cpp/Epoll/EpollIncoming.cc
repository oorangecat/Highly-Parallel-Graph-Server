//
// Created by marco on 07/05/2023.
//
#include "EpollConnection.hh"
#include "EpollIncoming.hh"
#include <unistd.h>


EpollIncoming::EpollIncoming(int t_fd, EpollInstance *inst, MessageQueue<int> *inq) {

	this->t_fd = t_fd;
	this->inst = inst;
	this->inqueue = inq;

}

EpollIncoming::~EpollIncoming(){

};

bool EpollIncoming::handleEvent(uint32_t events){

	int rec;

	if ((events & EPOLLERR) || (events & EPOLLHUP) || !(events & EPOLLIN)) {
		return false;
	} else {
		//ssize_t count = read(this->t_fd, &rec, sizeof(rec));	//PIPE approach discarded

		rec = this->inqueue->pop();		//TODO check performance, includes blocking op for mutex

		conn_t newconn;
		newconn.cfd = rec;

		EpollConnection *newepconn = new EpollConnection(&newconn);

		this->inst->registerEpollEntry(*newepconn);			//Registers new connection in the thread poll

	}
	return true;
};