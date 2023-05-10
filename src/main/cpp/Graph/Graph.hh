//
// Created by marco on 07/05/2023.
//

#ifndef SERVER_GRAPH_HH
#define SERVER_GRAPH_HH
#include "Walk.hh"
#include "Location.hh"
#include "../Config.hh"

using rtree = boostg::index::rtree<Location, boostg::index::quadratic<RTREENODES>>;

class Graph {
		rtree locTree;

};


#endif //SERVER_GRAPH_HH
