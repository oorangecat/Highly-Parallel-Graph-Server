//
// Created by marco on 25/05/2023.
//

#ifndef SERVER_CONNECTION_HH
#define SERVER_CONNECTION_HH

typedef struct conn{
		int cfd;
		std::vector<Edge*> buffer;
		int len = 0;
		std::mutex connmut;
}conn_t;

#endif //SERVER_CONNECTION_HH
