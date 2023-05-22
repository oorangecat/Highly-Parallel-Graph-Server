//
// Created by marco on 21/05/2023.
//

#include "QuadTree.hh"

// Insert a node into the quadtree
void QuadTree::insert(Node* node)
{
	if (node == NULL)
		return;

	// Current quad cannot contain it
	if (!inBoundary(node->pos))
		return;

	// We are at a quad of unit area
	// We cannot subdivide this quad further
	if (abs(topLeft.x - botRight.x) <= 1
			&& abs(topLeft.y - botRight.y) <= 1) {
		if (n == NULL)
			n = node;
		return;
	}

	if ((topLeft.x + botRight.x) / 2 >= node->pos.x) {
		// Indicates topLeftTree
		if ((topLeft.y + botRight.y) / 2 >= node->pos.y) {
			if (topLeftTree == NULL)
				topLeftTree = new QuadTree(
								Point(topLeft.x, topLeft.y),
								Point((topLeft.x + botRight.x) / 2,
											(topLeft.y + botRight.y) / 2));
			topLeftTree->insert(node);
		}

			// Indicates botLeftTree
		else {
			if (botLeftTree == NULL)
				botLeftTree = new QuadTree(
								Point(topLeft.x,
											(topLeft.y + botRight.y) / 2),
								Point((topLeft.x + botRight.x) / 2,
											botRight.y));
			botLeftTree->insert(node);
		}
	}
	else {
		// Indicates topRightTree
		if ((topLeft.y + botRight.y) / 2 >= node->pos.y) {
			if (topRightTree == NULL)
				topRightTree = new QuadTree(
								Point((topLeft.x + botRight.x) / 2,
											topLeft.y),
								Point(botRight.x,
											(topLeft.y + botRight.y) / 2));
			topRightTree->insert(node);
		}

			// Indicates botRightTree
		else {
			if (botRightTree == NULL)
				botRightTree = new QuadTree(
								Point((topLeft.x + botRight.x) / 2,
											(topLeft.y + botRight.y) / 2),
								Point(botRight.x, botRight.y));
			botRightTree->insert(node);
		}
	}
}

// Find a node in a quadtree
Node* QuadTree::search(Point p)
{
	// Current quad cannot contain it
	if (!inBoundary(p))
		return NULL;

	// We are at a quad of unit length
	// We cannot subdivide this quad further
	if (n != NULL)
		return n;

	if ((topLeft.x + botRight.x) / 2 >= p.x) {
		// Indicates topLeftTree
		if ((topLeft.y + botRight.y) / 2 >= p.y) {
			if (topLeftTree == NULL)
				return NULL;
			return topLeftTree->search(p);
		}

			// Indicates botLeftTree
		else {
			if (botLeftTree == NULL)
				return NULL;
			return botLeftTree->search(p);
		}
	}
	else {
		// Indicates topRightTree
		if ((topLeft.y + botRight.y) / 2 >= p.y) {
			if (topRightTree == NULL)
				return NULL;
			return topRightTree->search(p);
		}

			// Indicates botRightTree
		else {
			if (botRightTree == NULL)
				return NULL;
			return botRightTree->search(p);
		}
	}
};

// Check if current quadtree contains the point
bool QuadTree::inBoundary(Point p)
{
	return (p.x >= topLeft.x && p.x <= botRight.x
					&& p.y >= topLeft.y && p.y <= botRight.y);
}


// Helper function to calculate the distance between two points
double calculateDistance(Point p1, Point p2)
{
	double dx = p2.x - p1.x;
	double dy = p2.y - p1.y;
	return sqrt(dx * dx + dy * dy);
}

// Recursive function to collect points closer than MAXDIST
void QuadTree::collectPointsCloserThanMaxDist(Point p, double maxDist, std::vector<Node*>& result)
{
	if (!inBoundary(p))
		return;

	if (n != NULL) {
		double dist = calculateDistance(p, n->pos);
		if (dist <= maxDist)
			result.push_back(n);
	}

	if (topLeftTree != NULL)
		topLeftTree->collectPointsCloserThanMaxDist(p, maxDist, result);

	if (botLeftTree != NULL)
		botLeftTree->collectPointsCloserThanMaxDist(p, maxDist, result);

	if (topRightTree != NULL)
		topRightTree->collectPointsCloserThanMaxDist(p, maxDist, result);

	if (botRightTree != NULL)
		botRightTree->collectPointsCloserThanMaxDist(p, maxDist, result);
}

// Function to get points closer than MAXDIST
std::vector<Node*> QuadTree::getPointsCloserThanMaxDist(Point p)
{
	std::vector<Node*> result;
	collectPointsCloserThanMaxDist(p, MAXDIST, result);
	return result;
}