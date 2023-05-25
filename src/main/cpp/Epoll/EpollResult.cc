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

void writeAnswer(Response serializedStr, conn_t *conn) {
#if DEBUG==true
	std::cout<<"Writing answer to conn: "<<conn->cfd<<std::endl;
#endif
	std::string serialized = serializedStr.SerializeAsString();
	uint32_t size = htonl(serialized.size());  // Convert size to big-endian

	// Write the size in big-endian format
	conn->connmut.lock();
	ssize_t bytesWritten = write(conn->cfd, &size, sizeof(size));
	if (bytesWritten != sizeof(size)) {
		std::cerr << "Error writing size to file descriptor." << std::endl;
		//return;
	}

	// Write the serialized string
	bytesWritten = write(conn->cfd, serialized.c_str(), serialized.size());
	if (bytesWritten != serialized.size()) {
		std::cerr << "Error writing serialized string to file descriptor." << std::endl;
		return;
	}
	conn->connmut.unlock();
}

bool EpollResult::handleEvent(uint32_t events) {

	//cancel event

	uint64_t tmp;
	read(this->get_fd(), &tmp, sizeof(tmp));


	if ((events & EPOLLERR) || (events & EPOLLHUP) || !(events & EPOLLIN)) {
		return false;
	} else {

		Result **res = this->resQueue->pop();


		if(res!=nullptr){
			Message *msg = (*res)->getMsg();
			conn_t* conn = msg->get_conn(); //connection to be answered to

			Response response;

#if DEBUG == true
			std::cout<<"Received result ONETOONE: "<<(*res )->getShortestPath() <<" ONETOALL: "<<(*res)->getTotalLen()<<std::endl;
#endif

			if(!(*res)->getStatus()){
				response.set_status(Response_Status_ERROR);
			} else {
				response.set_status(Response_Status_OK);
			}
			//	if ((*res)->getTotalLen() != 0) {
					response.set_total_length((*res)->getTotalLen());
			//	} else if ((*res)->getShortestPath() != 0) {
					response.set_shortest_path_length((*res)->getShortestPath());
			//	}

			writeAnswer(response,conn);



			//TODO handle errorstr

		}
	}
	return true;

}

