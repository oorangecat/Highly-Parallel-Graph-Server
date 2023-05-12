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

class Message {
		bool RESET = false;

		std::vector<Edge> walks;
		Node *source;
		std::vector<Node*> dest;
public:
		Message(std::vector<Edge> walks, Node *source, std::vector<Node*> dest){
			this->walks=walks; this->source=source; this->dest=dest;
		}

		Message(std::vector<Edge> walks, Node *source, std::vector<Node*> dest, bool res){
			this->walks=walks; this->source=source; this->dest=dest;
			this->RESET=res;
		}

		Message() = default;

		Message(std::vector<Edge> walks, Node *source, Node* dest){
			this->walks=walks; this->source=source, this->dest.push_back(dest);
		}




			std::vector<Edge> getWalks(){ return this->walks;};
		std::vector<Node*> getDest(){ return this->dest;};
		void addWalk(Edge e) { this->walks.push_back(e);};
		void addDest(Node* n) { this->dest.push_back(n);};
		void setSource(Node* n) {this->source = n;};
		Node* getSource(){ return this->source;};
		bool isToMany(){ return this->dest.size() != 1;};

};



#endif //SERVER_MESSAGE_HH
