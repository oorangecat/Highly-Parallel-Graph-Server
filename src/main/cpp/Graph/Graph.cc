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
		res =	newinst->addPoint(el->getA());
		newinst->addPoint(el->getB());
		res->addEdge(el);
	}


	this->pointmap = newinst;			//Swap map
	urcu_qsbr_synchronize_rcu();		//Wait for graceperiod to end before deleting old map
	delete(oldmap);

	this->writelock.unlock();


}


void Graph::addLocation(Node *nnode){
	this->pointmap->addPoint(nnode);
}


uint32_t Graph::closestDistance(Node* loc) {
	this->pointmap->closestDistance(loc);
}

Node* Graph::closestPoint(Node *p){
	return this->pointmap->closestPoint(p);
}