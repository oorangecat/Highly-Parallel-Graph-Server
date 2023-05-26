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


EpollConnection::EpollConnection(conn_t *cfd, MessageQueue<Message*> *outqueue, MessageQueue<Result*> *retqueue, Graph *g) {
	this->cnn = cfd;
	this->outq = outqueue;
	this->retq = retqueue;
	this->set_fd(this->cnn->cfd);
	this->set_events(EPOLLIN);
	this->graph = g;
}

EpollConnection::~EpollConnection() noexcept {
	close(this->cnn->cfd);
	delete(this->cnn);
}
int type=0;
bool EpollConnection::writeAnswer(Response serializedStr, conn_t *conn) {
#if DEBUG==true
	std::cout<<"Writing answer to conn: "<<conn->cfd<<" type: "<<type<<std::endl;
#endif
	fflush(stdout);
	std::string serialized = serializedStr.SerializeAsString();

	fflush(stdout);
	uint32_t size = htonl(serialized.size());  // Convert size to big-endian

	// Write the size in big-endian format
	ssize_t bytesWritten = write(conn->cfd, &size, sizeof(size));
	if (bytesWritten != sizeof(size)) {
		std::cerr << "Error writing size to file descriptor:" <<  std::strerror(errno)<< std::endl;
		//return;
	}

	// Write the serialized string
	bytesWritten = write(conn->cfd, serialized.c_str(), serialized.size());
	if (bytesWritten != serialized.size()) {
		std::cerr << "Error writing serialized string to file descriptor:" <<  std::strerror(errno)<< std::endl;
		return false;
	}

	fflush(stdout);
	return true;
}


bool EpollConnection::handleEvent(uint32_t events) {
#if DATADEBUG == true
	std::cout << "Some data received on connection: " << this->cnn->cfd << std::endl;
#endif

	uint32_t msgSize;
	int count;

	if ((events & EPOLLERR) || (events & EPOLLHUP) || !(events & EPOLLIN)) {
		delete(this);
		return false;

	} else {
		count = read(this->cnn->cfd, &msgSize, sizeof(msgSize));

		if (count < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				return true;
			} else {
				throw std::runtime_error(
								std::string("connection message size read: ") + std::strerror(errno));
			}
		} else if (count == 0) {
			return false;
		}

		msgSize = ntohl(msgSize);

#if DATADEBUG == true
		std::cout<<msgSize<<" bytes of data received on connection "<<this->cnn->cfd<<std::endl;
#endif
		std::vector<char> buff(msgSize + 1);

		uint32_t totalRead = 0;
		while (totalRead < msgSize) {
			count = read(this->cnn->cfd, buff.data() + totalRead, msgSize - totalRead);

			if (count <= 0) {
				break;
			}
			totalRead += count;
		}


#if DATADEBUG == true
		std::cout <<"Actual read "<<totalRead<<" bytes on connection "<<this->cnn->cfd<<std::endl;
#endif

		if (count < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				return true;
			} else {

				throw std::runtime_error(
								std::string("connection message  read: ") + std::strerror(errno));
			}
		} else if (count == 0) {
			close(this->get_fd());
			return false;
		}


		Request req = parseProtobuf(&buff, msgSize);
		Response response;

		if (req.has_walk()) {                  //WALK ROUTE
#if DATADEBUG == true
			printf("Received WALK on connection %d\n", this->cnn->cfd);
			fflush(stdout);
#endif

			std::vector<Edge *> edges = parseEdges(req);
			for (auto e: edges)
				this->cnn->buffer.push_back(e);

			response.set_status(Response_Status_OK);
			type=1;


		} else if (req.has_onetoone()) {      //ONE-TO-ONE ROUTE

#if DEBUG == true
			std::cout << "Received ONE-TO-ONE on connection " << this->cnn->cfd << " | queue: " << this->outq->get_fd()
								<< std::endl;
#endif

			OneToOne msg = req.onetoone();

			if (this->cnn->buffer.size() > 0) {
				std::vector<Edge *> buff(this->cnn->buffer);

				this->cnn->buffer.clear();
				this->graph->addWalkVector(buff);
			}
			Node *s = this->graph->closestPoint(msg.origin().x(), msg.origin().y());
			Node *e = this->graph->closestPoint(msg.destination().x(), msg.destination().y());
			if (s != nullptr && e != nullptr) {

				uint64_t shortest = graph->shortestToOne(s, e);
				response.set_status(Response_Status_OK);
				response.set_shortest_path_length(shortest);
			} else {
				response.set_status(Response_Status_ERROR);
			}
			type=2;

		} else if (req.has_onetoall()) {      //ONE-TO-ALL ROUTE
#if DEBUG == true
			printf("Received one-to-all on connection %d\n", this->cnn->cfd);
			fflush(stdout);
#endif
			OneToAll msg = req.onetoall();
			type=3;
			if (this->cnn->buffer.size() > 0) {
				std::vector<Edge *> buff(this->cnn->buffer);

				this->cnn->buffer.clear();
				this->graph->addWalkVector(buff);
			}

			Node *s = this->graph->closestPoint(msg.origin().x(), msg.origin().y());
			if (s != nullptr) {

				uint64_t shortest = graph->shortestToAll(s);
				response.set_status(Response_Status_OK);
				response.set_total_length(shortest);
			} else {
				response.set_status(Response_Status_ERROR);
			}
			writeAnswer(response, this->cnn);          //closing connectiona after one-to-all
			//return false;
		} else {

			return false;
		}

		if (writeAnswer(response, this->cnn)){
			return true;
		}else {
			return false;
		}
	}
}


std::vector<Edge*> parseEdges(Request req){


	Walk walk = req.walk();
	int n = walk.locations_size();

	std::vector<Edge*> res;

	Location loc1, loc2;
	Node *prev = new Node(walk.locations(0).x(), walk.locations(0).y());
	for(int i = 1; i < n; i++) {
		loc2 = walk.locations(i);
		Node *b = new Node(loc2.x(), loc2.y());
		Edge *e = new Edge(prev,b,walk.lengths(i-1));
		prev = b;
		res.push_back(e);
	}

	return res;

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