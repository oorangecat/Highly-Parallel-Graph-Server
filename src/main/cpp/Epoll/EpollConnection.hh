//
// Created by marco on 07/05/2023.
//

/*
 * Represent a single connection, inside a NetWorker, handled by epoll
 * Handles reading of messages with protobuf and eventually handles the new walk or the OneToSomething req
 * Reset request is escalated via a special
 */

#ifndef SERVER_EPOLLCONNECTION_HH
#define SERVER_EPOLLCONNECTION_HH

#include "../Config.hh"
#include "EpollEntry.hh"
#include "../Graph/Message.hh"
#include "../Graph/Node.hh"
#include "../Graph/Edge.hh"
#include "MessageQueue/MessageQueue.hh"
#include "../Graph/Result.hh"
#include "../proto/locations.pb.h"

#include <vector>



typedef struct conn{
		int cfd;
		std::vector<Edge*> buffer;
		int len = 0;
}conn_t;


class EpollConnection : public EpollEntry{
		conn_t *cnn;
		MessageQueue<Message*> *outq;
		MessageQueue<Result*> *retq;
		Request parseProtobuf(std::vector<char> *buff, int count);

public:
		EpollConnection(conn_t *cfd, MessageQueue<Message*> *outqueue, MessageQueue<Result*> *retqueue);
		~EpollConnection();
		bool handleEvent(uint32_t events);

};


#endif //SERVER_EPOLLCONNECTION_HH
