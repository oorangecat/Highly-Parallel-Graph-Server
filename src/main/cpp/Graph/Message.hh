//
// Created by marco on 11/05/2023.
//

/*
 * Defines message from NetWorker to GraphWorker containing the request
 */

#ifndef SERVER_MESSAGE_HH
#define SERVER_MESSAGE_HH

#include "Edge.hh"
#include "Node.hh"
#include "../Config.hh"
#include "Result.hh"
#include "../Epoll/MessageQueue/MessageQueue.hh"
#include "../Epoll/Connection.hh"




class Message {
		bool RESET = false;

		conn_t *conn;
		std::vector<Edge*> walks;
		Node *source;
		std::vector<Node*> dest;

		MessageQueue<Result*> *retqueue;		//used to return result when ready

public:
		Message(conn_t *cfd, std::vector<Edge*> walks, Node *source, std::vector<Node*> dest,	MessageQueue<Result*> *retq ){
			this->walks = walks; this->source = source; this->dest = dest; this->retqueue = retq;
		}

		Message(conn_t *cfd, std::vector<Edge*> walks, Node *source, std::vector<Node*> dest, 	MessageQueue<Result*> *retq, bool res){
			this->walks = walks; this->source = source; this->dest = dest; this->retqueue = retq;
			this->RESET = res;this->conn = cfd;
		}

		Message() = default;

		Message(conn_t *cfd, std::vector<Edge*> walks, Node *source, 	MessageQueue<Result*> *retq) {
			this->conn = cfd; this->walks = walks; this->source = source; this->retqueue = retq;
		}

		Message(conn_t *cfd, bool res) {
			this->conn = cfd; this->RESET = res;
		}


		Message(conn_t *cfd, std::vector<Edge*> walks, Node *source, Node* dest, 	MessageQueue<Result*> *retq){
			this->conn = cfd; this->walks = walks; this->source=source, this->dest.push_back(dest); this->retqueue=retq;
		}



		std::vector<Edge*> getWalks(){ return this->walks;};
		std::vector<Node*> getDest(){ return this->dest;};
		void addWalk(Edge* e) { this->walks.push_back(e);};
		void addDest(Node* n) { this->dest.push_back(n);};
		void setSource(Node* n) {this->source = n;};
		Node* getSource(){ return this->source;};
		bool isToMany(){ return this->dest.size() != 1;};
		int get_cfd(){return this->conn->cfd;};
		conn_t *get_conn(){return this->conn;};
		MessageQueue<Result*> *getResQueue(){ return this->retqueue;};

};



#endif //SERVER_MESSAGE_HH
