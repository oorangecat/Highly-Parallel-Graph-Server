//
// Created by marco on 07/05/2023.
//

#include "Graph.hh"


void Graph::addWalkUnsync(Edge newWalk){
	Node* res =	this->pointmap->addPoint(newWalk.getA());
	this->addLocation(newWalk.getB());
	res->addEdge(&newWalk);
}



//MERGE EDGES
/*
 * 1.Copy whole vectors
 * 2.Merge new values
 * 3.Swap new vector
 * TODO check performances. Given the high number of walks, RW could be faster
 * */
void Graph::addWalkVector(std::vector<Edge*> walks) {
#if DEBUG==true
	printf("\nWRITE PROTECTED | Adding %ld walks to graph\n\n", walks.size());
	fflush(stdout);
#endif

#if USERCU==true

	this->writelock.lock();		//stop parallel writes

	//clone map
	std::unordered_map<int64_t, Node*>* newmap = new std::unordered_map<int64_t, Node*>();
	rcu_read_lock_qsbr(); 			//Read lock for copying old map

	std::unordered_map<int64_t, Node*>* oldmap = this->pointmap->getMap();
	newmap->insert(oldmap->begin(), oldmap->end());
	urcu_qsbr_read_unlock();		//map copied, release read lock
	urcu_qsbr_quiescent_state();

	PointMap *newinst = new PointMap(newmap);

	//insert new walks and locations
	Node *res;

	for(auto el : walks) {
		res =	newinst->addPoint(el->getA());				//TODO edges may be added to in-use nodes!
		newinst->addPoint(el->getB());
		res->addEdge(el);
	}


	this->pointmap = newinst;			//Swap map

	urcu_qsbr_synchronize_rcu();		//Wait for graceperiod to end before deleting old map
	delete(oldmap);

	this->writelock.unlock();
#elif USERCU == false
	pthread_rwlock_wrlock(&(this->rwlock));

	Node *res;
	for(auto el : walks) {
		res =	this->pointmap->addPoint(el->getA());				//TODO edges may be added to in-use nodes!
		this->pointmap->addPoint(el->getB());
		res->addEdge(el);
	}

	pthread_rwlock_unlock(&(this->rwlock));

#endif

}


void Graph::addLocation(Node *nnode){
	this->pointmap->addPoint(nnode);
}


uint32_t Graph::closestDistance(Node* loc) {
	return this->pointmap->closestDistance(loc);
}

Node* Graph::closestPoint(Node *p){
	return this->pointmap->closestPoint(p);
}

struct NodeWrapper {
		Node *node;
		int f_score;			//estimate to the end (g+distance)
		int g_score;			//from start

		NodeWrapper(Node *n, int g, int f) : node(n), f_score(f), g_score(g) {}

		bool operator<(const NodeWrapper other) const {
			return f_score > other.f_score;
		}
};

uint64_t Graph::shortestToOne(Node *source, Node *dest){
#if USERCU==true
	rcu_read_lock_qsbr(); 			//Read lock for copying old map
#elif USERCU == false
	pthread_rwlock_rdlock( &( this->rwlock ) );			//readlock on Graph
#endif

	//TODO query cache before

	/*
	 * G_score: cost FROM the starting point
	 * F_score: heuristic TO the final point 	(direct distance used)
	 */

	//std::unordered_set<Node*> open_set;			//Nodes to be evaluated
	std::priority_queue<NodeWrapper*> open_set;

	std::unordered_set<Node*>	closed_set;		//Nodes already evaluated

	std::unordered_map<Node*, Node*> parent;	//predecessor of node on the path
	//Node* s = this->pointmap->addPoint(source);
														//NODE   gscore	       fscore
	open_set.push( new NodeWrapper(source,   0, 		source->distance(dest)) );
	//TODO dynamic allocation may cause performance issue and memory leaks

	NodeWrapper *currentw;
	std::unordered_map<uint64_t, Edge*> *edgelist;
	uint64_t ret = 0;				//0 for failure

	while(!open_set.empty()){				//while something still to evaluate
		currentw = open_set.top();					//extract top element of the list

#if DEBUG==true

		printf( "SHORTEST| Current node: %ld | Edge length: %d\n", currentw->node->hash(), currentw->node->getEdges()->size() );
		fflush(stdout);
#endif

		if( currentw->node == dest ) {
			ret = sumPath(&parent, source, dest);			//Sum all lengths
			break;
		}

		open_set.pop();

		if( closed_set.find(currentw->node) != closed_set.end() ) {
			delete(currentw);
			continue;
		}

		closed_set.insert(currentw->node);			//curent node as already visited

		edgelist = currentw->node->getEdges();
		Edge* e;
		int32_t f;

		for (  auto pair : *edgelist ) {

			e = pair.second;
#if DEBUG==true
			printf("Evaluating edge %ld:%ld cost: %ld\n", e->getA()->hash(), e->getB()->hash(), e->getDist());
			fflush(stdout);
#endif
			f = e->getB()->distance( dest ) + currentw->g_score + e->getDist();

			parent.insert(std::make_pair(e->getB(), currentw->node));		//current node is father of neighbour
			open_set.push ( new NodeWrapper( e->getB(), currentw->g_score + e->getDist(), f));
		}
		delete(currentw);
	}

#if USERCU==true
	urcu_qsbr_read_unlock();
	urcu_qsbr_quiescent_state();
#elif USERCU==false
	pthread_rwlock_unlock(&(this->rwlock));
#endif

	//TODO there is an error in hashing!

#if DEBUG==true
	std::cout<<"shortest_path_ONETOONE "<< source->hash() << " : "<< dest->hash() << " = " << ret << std::endl;
#endif

	return ret;
}



uint64_t Graph::sumPath(std::unordered_map<Node*, Node*> *parent, Node* source, Node* dest){

	Node* next = dest;
	Node* prev;

	uint64_t sum = 0;
	int32_t dist;


	while( next != source){
		auto res = parent->find(next);
		if( res != parent->end()){
			prev = next;
			next = res->second;
			dist = next->edgeDist(prev);
			if(dist == -1)
				return 0;
			else
				sum += dist;
		} else{
			return 0;
		}
	}

	return sum;

}


Node* Graph::addPoint(Node *p){
	return this->pointmap->addPoint(p);
}
