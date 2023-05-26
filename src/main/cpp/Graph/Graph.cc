//
// Created by marco on 07/05/2023.
//

#include <limits>
#include <stack>
#include "Graph.hh"





//MERGE EDGES
/*
 * 1.Copy whole vectors
 * 2.Merge new values
 * 3.Swap new vector
 * */
void Graph::addWalkVector(std::vector<Edge*> walks) {
#if DEBUG==true
	printf("\nWRITE PROTECTED | Adding %ld walks to graph | Starting size %d\n\n", walks.size(), this->pointmap->size());
	fflush(stdout);
#endif


	pthread_rwlock_wrlock(&(this->rwlock));

	Node *a, *b;
	for(auto el : walks) {			//TODO optimize, reuse previous
		a =	this->pointmap->addPoint(el->getA());
		b = this->pointmap->addPoint(el->getB());
		if(a!=el->getA())
			delete(el->getA());

		el->setA(a);
		el->setB(b);
		a->addEdge(el);
	}

	this->cache->clear();
	pthread_rwlock_unlock(&(this->rwlock));
#if DEBUG==true
	printf("\nAdding WRITE PROTECTED  done. Size now: %d\n\n", this->pointmap->size());
	fflush(stdout);
#endif


}


void Graph::addLocation(Node *nnode){
	this->pointmap->addPoint(nnode);
}

Node* Graph::closestPoint(Node *p){
	return this->pointmap->closestPoint(p->x_(), p->y_());
}

Node* Graph::closestPoint(uint32_t x, uint32_t y) {
	return this->pointmap->closestPoint(x, y);
}


#if STUPIDSHORTEST==false

struct NodeDistanceComparator {
		bool operator()(Node* a, Node* b) const {
			return a->getDistance() > b->getDistance();
		}
};


uint64_t Graph::shortestToOne(Node *s, Node *d){

	Node *source = this->pointmap->closestPoint( s->x_(), s->y_() );
	Node *dest = this->pointmap->closestPoint( d->x_(), d->y_() );

	auto cacheret=this->cache->find((int64_t)source & (int64_t)dest);
	if(cacheret != this->cache->end()) {
		return cacheret->second;
	}
	pthread_rwlock_rdlock( &( this->rwlock ) );			//readlock on Graph

	//TODO query cache before

	unordered_map<Node*, uint64_t> distance;
	unordered_map<Node*, Node*> previous;
	priority_queue<Node*, std::vector<Node*>, NodeDistanceComparator> queue;


	unordered_map<Node*, Edge*> *edges;

	distance[source] = 0;
	source->setDistance(0);
	queue.push(source);
	Node* current;

	uint64_t dist;
	Node* neighbor;
	Node* tmp;

	while(!queue.empty()) {
		tmp = queue.top();
		queue.pop();
		current = this->pointmap->closestPoint(tmp->x_(), tmp->y_());

		/*if (current == dest)
			break;*/

		edges = current->getEdges();

		for (auto e: *edges) {
			neighbor = e.second->getB();
			dist = distance[current] + e.second->getDist();

			auto res = distance.find(neighbor);
			if (res != distance.end()) {
				if (res->second > dist) {
					distance[neighbor] = dist;
					previous[neighbor] = current;
					neighbor->setDistance(dist);
					queue.push(neighbor);
				}
			} else {
				distance[neighbor] = dist;
				neighbor->setDistance(dist);
				queue.push(neighbor);
			}
		}
	}



	pthread_rwlock_unlock(&(this->rwlock));

	uint64_t retval = dest->getDistance();
#if DEBUG==true
	std::cout<<"shortest_path_ONETOONE "<< source->hash() << " : "<< dest->hash() << " = " << retval << "\t Points: "<<this->pointmap->size()<<std::endl;
#endif

	this->cache->insert(std::make_pair((int64_t)source & (int64_t)dest, retval));
	return retval;
}




#elif STUPIDSHORTEST == true
uint64_t Graph::shortestToOne(Node *source, Node *dest){
#if DEBUG== true
	std::cout<<"Starting one-to-one... Edges: "<<this->pointmap->totEdges()<<std::endl;
#endif
	// Stack to store the nodes to visit
	std::stack<Node*> nodeStack;

	// Map to store the visited nodes
	std::unordered_map<Node*, bool> visited;

	// Map to store the distances from the start node to each node
	std::unordered_map<Node*, uint64_t> distances;

	Node* s = this->pointmap->closestPoint(source->x_(), source->y_());
	Node* d = this->pointmap->closestPoint(dest->x_(), dest->y_());


	// Push the start node onto the stack
	nodeStack.push(s);
	distances[s] = 0;

	while (!nodeStack.empty()) {
		// Pop the current node from the stack
		Node* tmp = nodeStack.top();
		Node* currentNode = this->pointmap->closestPoint(tmp->x_(), tmp->y_());
		nodeStack.pop();

		// Mark the current node as visited
		visited[currentNode] = true;
		// Check if the current node is the end node
		if (currentNode == d ) {
			printf("Dest: %lu\n",distances[d]);
			continue;
			//return distances[d];
		}

		// Iterate over the edges of the current node
		for (auto entry : *(currentNode->getEdges())) {
			Edge* edge = entry.second;
			tmp = edge->getB();
			Node* neighborNode = this->pointmap->closestPoint(tmp->x_(), tmp->y_());

			auto res = visited.find(neighborNode);
			if(res == visited.end())
				visited[neighborNode] = false;

			// Check if the neighbor node has not been visited


				// Calculate the new distance from the start node to the neighbor node
				uint64_t newDistance = distances[currentNode] + static_cast<uint64_t>(edge->getDist());

				// Update the distance if it is shorter than the current distance
				auto res2 = distances.find(neighborNode);
				if(res2 != distances.end()) {
					if (newDistance < res2->second) {
						distances[neighborNode] = newDistance;
						visited[neighborNode] = false;
					}
				} else  {
					distances[neighborNode] = newDistance;
				}
				// Push the neighbor node onto the stack for further exploration
			if (!visited[neighborNode]) {
				nodeStack.push(neighborNode);
			}
		}
	}
#if DEBUG==true
	std::cout<<"shortest_path_ONETOONE "<< s->hash() << " : "<< d->hash() << " = " << distances[d] << "\t Points: "<<this->pointmap->size()<<std::endl;
	std::cout<<"Visited: "<< visited.size()<<std::endl;
#endif

#if DEBUG==true
	std::cout<<"shortest_path_ONETOONE "<< s->hash() << " : "<< d->hash() << " = max" << "\t Points: "<<this->pointmap->size()<<std::endl;
#endif
	// Return a large value to indicate that no path was found
	return std::numeric_limits<uint64_t>::max();
}
#endif








uint64_t Graph::shortestToAll(Node *source) {

	pthread_rwlock_rdlock( &( this->rwlock ) );			//readlock on Graph

#if TOALLDEBUG==true
	std::cout<<"to-all-source: "<<source<<std::endl;
#endif

	std::unordered_map<Node*, uint64_t> currentNodes;	//for node distances
	std::priority_queue<Edge*> nextEdges;

	currentNodes.insert(std::make_pair(source, 0));

	std::unordered_map<Node*, Edge*> *edges = source->getEdges();
	Edge *e;
	Node *a, *b;

	for(auto p : *edges){
		nextEdges.push(p.second);
	}

	while (!nextEdges.empty()){
		e = nextEdges.top();
		nextEdges.pop();
		a = e->getA(); b = e->getB();

		auto res = currentNodes.find(b);
		auto adist = currentNodes.find(a);

		if(adist == currentNodes.end()){
			return 0;											//TODO throw error, graph broken
		}

		if(res == currentNodes.end()){			//if destination is not yet in the map, add it
			currentNodes[b] = adist->second + e->getDist();
			edges = b->getEdges();
			for(auto e: *edges)
				nextEdges.push(e.second);		//add all its edges

		} else {											//if we already have it in the map
			if(res->second > adist->second + e->getDist()){
				currentNodes[b] = adist->second + e->getDist();
				edges = b->getEdges();

				for(auto e: *edges)
					nextEdges.push(e.second);

			} else {													//Minimal edge did not improve anything
				continue;
			}
		}
	}

	pthread_rwlock_unlock(&(this->rwlock));

#if DEBUG == true
	uint64_t result = sumMap(&currentNodes);
	printf("shortest_path_ONETOALL %ld : %ld | Points: %d\n", source->hash(), result, this->pointmap->size());
	fflush(stdout);
	return result;
#elif  TOALLDEBUG == false
	return sumMap(&currentNodes);

#endif



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
			sum += dist;
		} else{
			return 0;
		}
	}

	return sum;

}

uint64_t Graph::sumMap(std::unordered_map<Node*,uint64_t> *map){
	uint64_t sum=0;
	for(auto e:*map){
		sum += e.second;
	}
	return sum;
}

Node* Graph::addPoint(Node *p){
	return this->pointmap->addPoint(p);
}


