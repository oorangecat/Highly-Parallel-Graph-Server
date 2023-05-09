
#include "Epoll/EpollServer.hh"
#include "Epoll/EpollInstance.hh"
#include "Epoll/MessageQueue/MessageQueue.hh"
#include "NetWorker.hh"
#include "Config.hh"

#include <thread>
#include <vector>

int main(int argc, char *argv[]) {



	MessageQueue<int> *netQueues[NETTHREADS];
	NetWorker *netw[NETTHREADS];
	vector<std::thread*> netth;
	EpollInstance epoll;
	std::thread *tmp;
	for(int i=0; i<NETTHREADS; i++){
		netQueues[i] = new MessageQueue<int>();
		netw[i] = new NetWorker(netQueues[i]);
		tmp = new std::thread(&NetWorker::threadMain, netw[i]);
		netth.push_back(tmp);
	}

	EpollServer server(1234,NETTHREADS, netQueues);

	epoll.registerEpollEntry(server);
	cout << "\nServer listening at port 1234\n";

	while(1){
		epoll.waitAndHandleEvents();
	}

	for(auto t = netth.begin(); t!=netth.end(); ++t){
		(*t)->join();
	}

	cout<<"hi";


}

