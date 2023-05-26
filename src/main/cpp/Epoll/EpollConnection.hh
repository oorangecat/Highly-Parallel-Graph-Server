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
#include "../Graph/Graph.hh"
#include "MessageQueue/MessageQueue.hh"
#include "../Graph/Result.hh"
#include "../proto/locations.pb.h"
#include "./Connection.hh"

#include <vector>
#include <mutex>






class EpollConnection : public EpollEntry{
		conn_t *cnn;
		Graph *graph;
		MessageQueue<Message*> *outq;
		MessageQueue<Result*> *retq;
		Request parseProtobuf(std::vector<char> *buff, int count);

		bool writeAnswer(Response serializedStr, conn_t *conn);

public:
		EpollConnection(conn_t *cfd, MessageQueue<Message*> *outqueue, MessageQueue<Result*> *retqueue, Graph *g);
		~EpollConnection();
		bool handleEvent(uint32_t events);

};


#endif //SERVER_EPOLLCONNECTION_HH
