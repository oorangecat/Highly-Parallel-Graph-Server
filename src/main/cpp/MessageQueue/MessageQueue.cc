//
// Created by marco on 09/05/2023.
//

#include "MessageQueue.hh"

#include <unistd.h>
#include <iostream>
#include <sys/eventfd.h>
#include <cstring>
#include <sys/epoll.h>

template <typename T>
MessageQueue<T>::MessageQueue() {
	this->efd = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);		//Creates event file descriptor for EpollUsage

	if (this->eventfd == -1) {
		throw std::runtime_error(
						std::string("listen connection: ") + std::strerror(errno));
		exit(1);
	}

}


template <typename T>
void MessageQueue<T>::push(const T& msg){
	this->mut.lock();
	this->mqueue.push(msg);
	this->mut.unlock();
	write(this->efd, EPOLLIN, sizeof(EPOLLIN));		//signals available data to epoll
}

template <typename T>
T MessageQueue<T>::pop(){
	this->mut.lock();
	T ret = this->mqueue.pop();
	this->mut.unlock();
	return ret;
}

template <typename T>
int MessageQueue<T>::get_fd(){
	return this->eventfd;
}