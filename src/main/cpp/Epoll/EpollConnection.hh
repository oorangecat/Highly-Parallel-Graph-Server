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
#include "../proto/locations.pb.h"
#include <vector>



typedef struct conn{
		int cfd;
		std::vector<char> buffer;		//TODO replace with new incoming walks
		int len = 0;
}conn_t;


class EpollConnection : public EpollEntry{
		conn_t *c;

		Request parseProtobuf(char *buff, int count);

public:
		EpollConnection(conn_t *cfd);
		~EpollConnection();
		bool handleEvent(uint32_t events);

};


#endif //SERVER_EPOLLCONNECTION_HH
