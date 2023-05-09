//
// Created by marco on 07/05/2023.
//

#ifndef SERVER_EPOLLSERVER_HH
#define SERVER_EPOLLSERVER_HH

#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "EpollEntry.hh"
#include "MessageQueue/MessageQueue.hh"




class EpollServer : public EpollEntry {

		short int port;
		struct sockaddr_in saddr;
		int sfd;

		int nthreads;
		//int *threads_fd;		//array of fd for threads communication		//PIPE approch discarded

		MessageQueue<int> **thqueues;
		int lastth = 0;

		int acceptConnection();
		bool writeToThread(int fd);
		//void voidThread();


public:
		EpollServer(uint16_t port, int nth, MessageQueue<int> **threads);
		~EpollServer();
		bool handleEvent(uint32_t events);



};


#endif //SERVER_EPOLLSERVER_HH
