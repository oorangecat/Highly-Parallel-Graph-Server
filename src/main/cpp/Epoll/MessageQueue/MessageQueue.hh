//
// Created by marco on 09/05/2023.
//

/*
 * FIFO queue used for inter-thread communication
 * between Main and Networkers and GraphWorkers
 */
#ifndef SERVER_MESSAGEQUEUE_HH
#define SERVER_MESSAGEQUEUE_HH
#include <queue>
#include <mutex>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <cstring>
#include <unistd.h>

template<class T>
class MessageQueue {

		std::queue<T> mqueue;
		std::mutex mut;
		int efd;

public:
		MessageQueue(){
			this->efd = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);		//Creates event file descriptor for EpollUsage

			if (this->efd == -1) {
				throw std::runtime_error(
								std::string("listen connection: ") + std::strerror(errno));
				exit(1);
			}
		}

		void push(T msg){
			this->mut.lock();
			this->mqueue.push(msg);
			this->mut.unlock();
			uint64_t ev = EPOLLIN;
			write(this->efd, &ev, sizeof(ev));		//signals available data to epoll
		}


		T pop(){

			this->mut.lock();
			T ret = this->mqueue.front();
			this->mqueue.pop();
			this->mut.unlock();
			return ret;
		}


		int get_fd(){

			return this->efd;
		}

};


#endif //SERVER_MESSAGEQUEUE_HH
