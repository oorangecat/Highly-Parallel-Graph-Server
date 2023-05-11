//
// Created by marco on 11/05/2023.
//

#ifndef SERVER_EPOLLGRAPHMESSAGE_HH
#define SERVER_EPOLLGRAPHMESSAGE_HH
#include <sys/epoll.h>
#include <EpollEntry.hh>
#include <EpollInstance.hh>
#include "../Graph/Message.hh"
#include "MessageQueue/MessageQueue.hh"

class EpollGraphMessage : public EpollEntry  {
		EpollInstance *inst;
		MessageQueue<Message> *inqueue;
public:
		EpollGraphMessage(EpollInstance *inst, MessageQueue<Message> *inq);
		~EpollGraphMessage();
		bool handleEvent(uint32_t events);
};


#endif //SERVER_EPOLLGRAPHMESSAGE_HH
