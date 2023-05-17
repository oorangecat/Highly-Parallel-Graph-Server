//
// Created by marco on 13/05/2023.
//

#ifndef SERVER_EPOLLRESULT_HH
#define SERVER_EPOLLRESULT_HH

#include <sys/epoll.h>
#include "EpollEntry.hh"
#include "MessageQueue/MessageQueue.hh"
#include "../Graph/Message.hh"
#include "../Graph/Result.hh"


class EpollResult : public EpollEntry {
	MessageQueue<Result*> *resQueue;

public:
		EpollResult(MessageQueue<Result*> *q);
		~EpollResult();
		bool handleEvent(uint32_t events);

};


#endif //SERVER_EPOLLRESULT_HH
