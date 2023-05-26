//
// Created by marco on 07/05/2023.
//


/*
 * Used by a NetWorker to check for new assigned connections
 * Reads the connection and adds it to the thread EpollInstance
 * Creates a relative EpollConnection
 */


#ifndef SERVER_EPOLLINCOMING_HH
#define SERVER_EPOLLINCOMING_HH

#include "EpollEntry.hh"
#include "EpollInstance.hh"
#include "MessageQueue/MessageQueue.hh"
#include <sys/epoll.h>
#include <vector>



class EpollIncoming : public EpollEntry {
		EpollInstance *inst;
		MessageQueue<int> *inqueue;
		Graph *graph;

public:
		EpollIncoming(EpollInstance *inst, MessageQueue<int> *inq,Graph *g);
		~EpollIncoming();
		bool handleEvent(uint32_t events);

};


#endif //SERVER_EPOLLINCOMING_HH
