//
// Created by marco on 13/05/2023.
//

#include "EpollResult.hh"

EpollResult::EpollResult(MessageQueue<Result*> *q){
	this->resQueue = q;
	this->set_fd(q->get_fd());
	this->set_events(EPOLLIN);
}



EpollResult::~EpollResult(){
	delete(this->resQueue);
};

bool EpollResult::handleEvent(uint32_t events) {
#if DEBUG == true
	printf("Received result from graphworker");
	fflush(stdout);
#endif
	if ((events & EPOLLERR) || (events & EPOLLHUP) || !(events & EPOLLIN)) {
		return false;
	} else {
		Result **res = this->resQueue->pop();
		if(res!=nullptr){
			Message *msg = (*res)->getMsg();
			int cfd = msg->get_cfd();							//fd of the connection to be answered to
			//TODO send back answer via Protobuf
		}
	}
	return true;

}

