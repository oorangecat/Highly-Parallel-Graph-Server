//
// Created by marco on 11/05/2023.
//
/*
 * Defines message sent back from GraphWorker to Networker
 * After request has been completed
 */

#ifndef SERVER_RESULT_HH
#define SERVER_RESULT_HH

class Message;			//circular dependency

class Result {

private:
		bool status; //true OK (0) false ERROR(1)
		Message* msg = nullptr;
		uint64_t shortest_path = -1;			//uint max
		uint64_t total_len = -1;
		char* errstr = nullptr;

public:

		Result() = default;

		void setStatus(bool s) {this->status=s;};
		void setMessage(Message* m){this->msg=m;};
		void setShortest(uint64_t s){this->shortest_path = s;};
		void setTotLen(uint64_t t){this->total_len=t;};
		void setErrStr(char* s){this->errstr=s;};

		bool getStatus()  {return status;}

		Message *getMsg()  {	return msg;	}

		uint64_t getShortestPath() {return this->shortest_path;}

		uint64_t getTotalLen()  {return this->total_len;	}

		char *getErrstr() {	return errstr;}



};


#endif //SERVER_RESULT_HH
