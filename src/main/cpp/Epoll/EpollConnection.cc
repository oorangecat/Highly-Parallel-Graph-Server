//
// Created by marco on 07/05/2023.
//

#include "EpollConnection.hh"
#include <sys/epoll.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>





EpollConnection::EpollConnection(conn_t *cfd) {
	this->c = cfd;
	this->set_fd(this->c->cfd);
	this->set_events(EPOLLIN);
}

EpollConnection::~EpollConnection() noexcept {

}

bool EpollConnection::handleEvent(uint32_t events) {
#if DEBUG == true
	std::cout << "Some data received on connection: " << this->c->cfd << std::endl;
#endif

	int msgSize, count;
	char *buff;
	if ((events & EPOLLERR) || (events & EPOLLHUP) || !(events & EPOLLIN)) {

		return false;

	} else {

			count = read(this->c->cfd, &msgSize, sizeof(msgSize));

			if(count < 0){
				throw std::runtime_error(
								std::string("connection message size read: ") + std::strerror(errno));
				exit(1);
			} else if (count == 0)
				return true;

			msgSize = ntohl(msgSize);

			buff = new char[msgSize+1];

			count = read(this->c->cfd, buff, msgSize);

#if DEBUG == true
			printf("Message size: %d\n",msgSize);
			fflush(stdout);
#endif

			if(count <= 0){
				throw std::runtime_error(
								std::string("connection message  read: ") + std::strerror(errno));
				exit(1);
			}

			Request req = parseProtobuf(buff, count);

			if(req.has_walk()){									//WALK ROUTE
#if DEBUG == true
				printf("Received walk on connection %d\n", this->c->cfd);
				fflush(stdout);
#endif
			} else if (req.has_onetoone()){			//ONE-TO-ONE ROUTE
#if DEBUG == true
				printf("Received one-to-one on connection %d\n", this->c->cfd);
				fflush(stdout);
#endif

			} else if (req.has_onetoall()){			//ONE-TO-ALL ROUTE
#if DEBUG == true
				printf("Received one-to-all on connection %d\n", this->c->cfd);
				fflush(stdout);
#endif
			}
			return true;
	}
}




Request EpollConnection::parseProtobuf(char *buff, int count){

	Request req;

	if (!req.ParseFromArray(buff,count )) {
		// Parse error
		std::cout << "Parsing Data error" << std::endl;
		exit(1);
	}
	std::cout << "Req size: "<<req.ByteSizeLong()<<std::endl;
	return req;

}