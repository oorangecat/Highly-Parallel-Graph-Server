#include "Graph/Edge.hh"
#include "Graph/Graph.hh"
#include "Graph/Node.hh"
#include "Epoll/EpollServer.hh"
#include "Epoll/EpollInstance.hh"
#include "Epoll/MessageQueue/MessageQueue.hh"
#include "NetWorker.hh"
#include "Config.hh"
#include "Graph/Message.hh"
#include "GraphWorker.hh"

/*
 * TODO Implement cache
 * TODO implement ProtoBuf answer
 */

#include <thread>
#include <vector>

int main(int argc, char *argv[]) {

#if GRAPHDEBUG == false

	Graph *graph = new Graph();

	MessageQueue<int> *netQueues[NETTHREADS];
	MessageQueue<Message*> *graphQueue = new MessageQueue<Message*>();
	NetWorker *netw[NETTHREADS];
	GraphWorker *graphw[GRAPHTTREADS];

	vector<std::thread*> netth;
	vector<std::thread*> graphth;

	EpollInstance epoll;
	std::thread *tmp;


	for(int i=0; i<GRAPHTTREADS; i++){
		graphw[i] = new GraphWorker(graphQueue, graph);
		tmp = new std::thread(&GraphWorker::threadMain, graphw[i]);
		graphth.push_back(tmp);
	}


	for(int i=0; i<NETTHREADS; i++){
		netQueues[i] = new MessageQueue<int>();
		netw[i] = new NetWorker(netQueues[i], graphQueue);
		tmp = new std::thread(&NetWorker::threadMain, netw[i]);
		netth.push_back(tmp);
	}

	EpollServer server(1222,NETTHREADS, netQueues);

	epoll.registerEpollEntry(server);
	cout << "\nServer listening at port 1222\n";

	while(1){
		epoll.waitAndHandleEvents();
	}

	for(auto t = netth.begin(); t!=netth.end(); ++t){
		(*t)->join();
	}
	delete(graph);
	cout<<"hi";

#elif GRAPHDEBUG == true
	Graph graph;
	Node n1(503, 401);
	Node n2(703, 443);
	Node n3(532, 111);
	Node n4(11111, 22222);

	graph.addLocation(&n1);
	graph.addLocation(&n2);
	graph.addLocation(&n3);
	graph.addLocation(&n4);

	Node n5(11112, 22224);
	Node n6(505, 440);
	Node n7(799, 500);
	Node n8(1, 1);

	cout<<"Dist: "<<graph.closestDistance(&n5)<< " "<<graph.closestDistance(&n6)<<" "<<graph.closestDistance(&n7)
	<<" "<<graph.closestDistance(&n8)<<endl;
	cout<<n6.distance(&n1)<<" "<<n6.distance(&n2)<<" "<<n6.distance(&n3)<<" "<<n6.distance(&n4)<<endl;
	cout<<n6.hash()<<" "<<n2.hash();
#endif

}

