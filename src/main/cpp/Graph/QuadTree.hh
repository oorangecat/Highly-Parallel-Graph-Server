//
// Created by marco on 21/05/2023.
//

#ifndef SERVER_QUADTREE_HH
#define SERVER_QUADTREE_HH

#include "./Node.hh"
#include <cmath>
#include <iostream>
#include "../Config.hh"
using namespace std;


//Implementation from https://www.geeksforgeeks.org/quad-tree/

class QuadTree {
		// Hold details of the boundary of this node
		Point topLeft;
		Point botRight;

		// Contains details of node
		Node* n;

		// Children of this tree
		QuadTree* topLeftTree;
		QuadTree* topRightTree;
		QuadTree* botLeftTree;
		QuadTree* botRightTree;

public:
		QuadTree(Point topL, Point botR)
		{
			n = NULL;
			topLeftTree = NULL;
			topRightTree = NULL;
			botLeftTree = NULL;
			botRightTree = NULL;
			topLeft = topL;
			botRight = botR;
		}
		QuadTree()
		{
			Point topL(0,0);
			Point botR(QUADTREELIM,QUADTREELIM);
			n = NULL;
			topLeftTree = NULL;
			topRightTree = NULL;
			botLeftTree = NULL;
			botRightTree = NULL;
			topLeft = topL;
			botRight = botR;
		}

		void insert(Node*);
		Node* search(Point);
		bool inBoundary(Point);
		void collectPointsCloserThanMaxDist(Point p, double maxDist, std::vector<Node*>& result);  //this one is made by ChatGPT
		std::vector<Node*> getPointsCloserThanMaxDist(Point p);																			 //this one is made by ChatGPT

};


#endif //SERVER_QUADTREE_HH
