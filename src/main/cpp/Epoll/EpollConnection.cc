//
// Created by marco on 07/05/2023.
//

#include "EpollConnection.hh"
#include <sys/epoll.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>

std::vector<Edge*> parseEdges(Request req);
std::vector<Node> parseMany(Request req);


EpollConnection::EpollConnection(conn_t *cfd, MessageQueue<Message*> *outqueue, MessageQueue<Result*> *retqueue) {
	this->cnn = cfd;
	this->outq = outqueue;
	this->retq = retqueue;
	this->set_fd(this->cnn->cfd);
	this->set_events(EPOLLIN);
}

EpollConnection::~EpollConnection() noexcept {
	delete(this->cnn);

}

bool EpollConnection::handleEvent(uint32_t events) {
#if DEBUG == true
	std::cout << "Some data received on connection: " << this->cnn->cfd << std::endl;
#endif

	uint32_t msgSize;
	int count;

	if ((events & EPOLLERR) || (events & EPOLLHUP) || !(events & EPOLLIN)) {

		return false;

	} else {

		count = read(this->cnn->cfd, &msgSize, sizeof(msgSize));

		if(count < 0){
			throw std::runtime_error(
							std::string("connection message size read: ") + std::strerror(errno));
			exit(1);
		} else if (count == 0)
			return false;

		msgSize = ntohl(msgSize);
		/*if (msgSize<0)
			return true;*/
#if DEBUG == true
		std::cout<<msgSize<<" bytes of data received on connection "<<this->cnn->cfd<<std::endl;
#endif
		std::vector<char> buff(msgSize+1);

		int totalRead = 0;
		while (totalRead < msgSize) {
			count = read(this->cnn->cfd, buff.data() + totalRead, msgSize - totalRead);
			if (count <= 0) {
				break;
			}
			totalRead += count;
		}

		//count = read(this->cnn->cfd, buff.data(), msgSize);

#if DEBUG == true
		std::cout <<"Actual read "<<totalRead<<" bytes on connection "<<this->cnn->cfd<<std::endl;
#endif

		if(count <= 0){
			throw std::runtime_error(
							std::string("connection message  read: ") + std::strerror(errno));
			exit(1);
		}

		Request req = parseProtobuf(&buff, msgSize);

		if(req.has_walk()){									//WALK ROUTE
#if DEBUG == true
				printf("Received WALK on connection %d\n", this->cnn->cfd);
				fflush(stdout);
#endif

				std::vector<Edge*> edges = parseEdges(req);
				for(auto e : edges)
					this->cnn->buffer.push_back(e);

			Message *graphmsg = new Message(this->cnn->cfd, false);
			Result *res = new Result();			//TODO delete result in epollresult
			res->setMessage(graphmsg);
			res->setStatus(true);
			this->retq->push(res);

			return true;

			} else if (req.has_onetoone()){			//ONE-TO-ONE ROUTE

#if DEBUG == true
			std::cout<<"Received ONE-TO-ONE on connection "<< this->cnn->cfd <<" | queue: "<< this->outq->get_fd()<<std::endl;
#endif

				OneToOne msg = req.onetoone();
				Node *s = new Node(msg.origin().x(), msg.origin().y());									//TODO check memory leak
				Node *e = new Node(msg.destination().x(), msg.destination().y());
				std::vector<Edge*> buff(this->cnn->buffer);

				Message *graphmsg = new Message(this->cnn->cfd, buff, s, e, this->retq);		//TODO check delete

				this->cnn->buffer.clear();			//clears Edge cache as they are sent to graphworker

				outq->push(graphmsg);				//Sent new message to graphworker

				return true;

			} else if (req.has_onetoall()){			//ONE-TO-ALL ROUTE
#if DEBUG == true
				printf("Received one-to-all on connection %d\n", this->cnn->cfd);
				fflush(stdout);
#endif
				OneToAll msg = req.onetoall();
				Node *s = new Node(msg.origin().x(), msg.origin().y());
				std::vector<Edge*> buff(this->cnn->buffer);

				Message *graphmsg = new Message(this->cnn->cfd, buff, s, this->retq);
				this->cnn->buffer.clear();

				outq->push(graphmsg);

				return true;

			} else
				return false;

	}
}


std::vector<Edge*> parseEdges(Request req){


	Walk walk = req.walk();
	int n = walk.locations_size();
	std::vector<Edge*> res;

	Location loc1, loc2;

	for(int i = 0; i < n-1; i++) {
		loc1 = walk.locations(i);
		loc2 = walk.locations(i+1);
		Node *a = new Node(loc1.x(), loc1.y());
		Node *b = new Node(loc2.x(), loc2.y());
		Edge *e = new Edge(a,b,walk.lengths(i));			//TODO check memory leak
		res.push_back(e);
	}

	return res;

}

std::vector<Node> parseMany(Request req){
	//TODO implement parsing many destinations
}

Request EpollConnection::parseProtobuf(std::vector<char> *buff, int count){

	Request req;

	if (!req.ParseFromArray(buff->data(), count )) {
		// Parse error
		std::cout << "Parsing Data error" << std::endl;
		//exit(1);
	}

	return req;

}