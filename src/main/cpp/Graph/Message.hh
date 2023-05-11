//
// Created by marco on 11/05/2023.
//

#ifndef SERVER_MESSAGE_HH
#define SERVER_MESSAGE_HH

#include "Edge.hh"
#include "Node.hh"

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

		std::vector<Edge> getWalks(){ return this->walks;};
		std::vector<Node*> getDest(){ return this->dest;};
		Node* getSource(){ return this->source;};
		bool isToMany(){ return dest.size() == 1;};

};



#endif //SERVER_MESSAGE_HH
