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

void writeAnswer(Response serializedStr, int cfd) {
#if DEBUG==true
	std::cout<<"Writing answer to conn: "<<cfd<<std::endl;
#endif
	std::string serialized;
	serializedStr.SerializeToString(&serialized);
	uint32_t size = htonl(serialized.size());  // Convert size to big-endian

	// Write the size in big-endian format
	ssize_t bytesWritten = write(cfd, &size, sizeof(size));
	if (bytesWritten != sizeof(size)) {
		std::cerr << "Error writing size to file descriptor." << std::endl;
		return;
	}

	// Write the serialized string
	bytesWritten = write(cfd, serialized.data(), serialized.size());
	if (bytesWritten != serialized.size()) {
		std::cerr << "Error writing serialized string to file descriptor." << std::endl;
		return;
	}
}

bool EpollResult::handleEvent(uint32_t events) {

	//cancel event
	uint64_t tmp;
	read(this->get_fd(), &tmp, sizeof(tmp));

#if DEBUG == true
	printf("Received result from graphworker\n");
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

			Response response;

			if(!(*res)->getStatus()){
				response.set_status(Response_Status_ERROR);
			} else {
				response.set_status(Response_Status_OK);
				if ((*res)->getTotalLen() != -1) {
					response.set_total_length((*res)->getTotalLen());
				} else if ((*res)->getShortestPath() != -1) {
					response.set_shortest_path_length((*res)->getShortestPath());
				}
			}
			writeAnswer(response,cfd);



			//TODO handle errorstr

		}
	}
	return true;

}

