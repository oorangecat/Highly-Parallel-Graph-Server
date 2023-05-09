//
// Created by marco on 07/05/2023.
//

#include "EpollConnection.hh"
#include <sys/epoll.h>


EpollConnection::EpollConnection(conn_t *cfd) {
	this->c = cfd;
	this->set_fd(cfd->cfd);
	this->set_events(EPOLLIN);

}

EpollConnection::~EpollConnection() noexcept {

}

bool EpollConnection::handleEvent(uint32_t events) {
#if DEBUG == true
	std::cout<<"Some data received on connection: "<<this->c->cfd;
#endif
}