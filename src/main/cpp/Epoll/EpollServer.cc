//
// Created by marco on 07/05/2023.
//

#include "EpollServer.hh"
#include "EpollConnection.hh"

#include <sys/epoll.h>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <iostream>



EpollServer::EpollServer(uint16_t port, int nth, int *threads) {
	this->port = port;
	this->threads_fd = threads;
	this->nthreads = nth;

	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family      = AF_INET;              // IPv4
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);    // Bind to all available interfaces
	saddr.sin_port        = htons(port);          // Requested port

	this->sfd = socket(AF_INET, SOCK_STREAM, 0);
	int flags = fcntl(sfd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	fcntl(sfd, F_SETFL, flags);

	int reuse = 1;
	if (setsockopt(this->sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		throw std::runtime_error(
						std::string("setsockopt for SO_REUSEADDR: ") + std::strerror(errno));
		exit(1);
	}

	if(bind(this->sfd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0 ){
		throw std::runtime_error(
						std::string("bind socket: ") + std::strerror(errno));
		exit(1);
	}

	int ret = listen(this->sfd, SOMAXCONN);

	if(ret<0){
		throw std::runtime_error(
						std::string("listen connection: ") + std::strerror(errno));
		exit(1);
	}
}



EpollServer::~EpollServer() {
	/*		PIPES approach discarded
	for(int i=0; i<nthreads; i++){
		close(threads_fd[i]);
	}*/
	delete(thqueues);
	delete(this);
}

int EpollServer::acceptConnection(){

	int cfd = accept(this->sfd, NULL, NULL);

	if(cfd >= 0){
		int flags = fcntl(cfd, F_GETFL, 0);
		flags |= O_NONBLOCK;
		fcntl(cfd, F_SETFL, flags);

		return cfd;

	} else {
		throw std::runtime_error(
						std::string("accept: ") + std::strerror(errno));
		exit(1);
	}
}

bool EpollServer::handleEvent(uint32_t events) {

		if ((events & EPOLLERR) || (events & EPOLLHUP) || !(events & EPOLLIN)) {
			return false;
		} else {

			int newConn = acceptConnection();

			if(newConn != NULL){


				/*if(!writeToThread(newConn)){			//PIPES approach discarded
					voidThread();
				}*/

			} else{
				throw std::runtime_error(
								std::string("socket: ") + std::strerror(errno));
				exit(1);
			}
	}
	return true;
}

//Sends the new connection fd to a thread
bool EpollServer::writeToThread(int fd){

/*	int *p = &fd;		//PIPES approach discarded
	if( !write(threads_fd[lastth], p, sizeof(fd) )){
		fprintf(stderr, "Error in writing on connection %d", lastth);
		return false;
	}*/

	this->thqueues[lastth]->push(fd);


	lastth = (lastth+1) % nthreads;
	return true;
}

/*		//PIPES approach discarded
//Deletes dead thread_fd from array

void EpollServer::voidThread(){
	close(threads_fd[lastth]);

	for(int i=lastth; i+1 < nthreads; i++){
		threads_fd[i] = threads_fd[i+1];
	}

	nthreads--;

	if(lastth >=nthreads)
		lastth = 0;
}*/
