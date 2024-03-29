//
// Created by marco on 11/05/2023.
//

#include "EpollGraphMessage.hh"
//TODO implement graph parallelism via RCU in Worker
/*
 * Threads are registered for RCU in GraphWorker
 */

EpollGraphMessage::EpollGraphMessage(EpollInstance *inst, MessageQueue<Message*> *inq, Graph *g) {

	this->inst = inst;
	this->inqueue = inq;
	this->set_fd(inq->get_fd());
	this->set_events(EPOLLIN);
	this->graph = g;

}



bool EpollGraphMessage::handleEvent(uint32_t events){

#if DEBUG == true
	printf("GraphThread %d received a new payload\n", this->id);
	fflush(stdout);
#endif

	//cancel event
	uint64_t tmp;
	read(this->get_fd(), &tmp, sizeof(tmp));

	if ((events & EPOLLERR) || (events & EPOLLHUP) || !(events & EPOLLIN)) {
		return false;
	} else {

		Message **tmp = this->inqueue->pop();

		Result *res = new Result();
		if(tmp!=nullptr){			//if thread manages to get the message first
			Message *rec = *tmp;

#if DEBUG == true
			printf("Message type: %c on connection %d\n", rec->isToMany() ? 'm':'s', rec->get_cfd());
			fflush(stdout);
#endif

			std::vector<Edge*> walks = rec->getWalks();
			if(walks.size()>0){
				this->graph->addWalkVector(walks);
			}

			if( !rec->isToMany() ) {          //ONE-TO-ONE route
				Node *source = this->graph->closestPoint(rec->getSource());
				Node *dest = this->graph->closestPoint(rec->getDest().front());
				uint64_t mindist = this->graph->shortestToOne(source, dest);

				res->setShortest(mindist);
				res->setStatus( !(mindist==0) );

			} else {														//ONE-TO-ALL route
				Node *source = this->graph->closestPoint(rec->getSource());
				uint64_t totdist = this->graph->shortestToAll(source);

				res->setTotLen(totdist);
				res->setStatus(!(totdist==0));
			}

			res->setMessage(rec);

#if DEBUG==true
			std::cout<<"RES : "<< res->getShortestPath()<<std::endl;
#endif
			rec->getResQueue()->push(res);			//send message back to netWorker
		}



	}
	return true;
};

EpollGraphMessage::~EpollGraphMessage(){}
