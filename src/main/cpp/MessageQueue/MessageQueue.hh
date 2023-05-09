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

template<typename T>
class MessageQueue {

		std::queue<T> mqueue;
		std::mutex mut;
		int efd;
public:
		MessageQueue();
		void push(const T& msg);
		T pop();
		int get_fd();

};


#endif //SERVER_MESSAGEQUEUE_HH
