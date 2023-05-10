//
// Created by marco on 10/05/2023.
//

#ifndef SERVER_WALK_HH
#define SERVER_WALK_HH


#include "Location.hh"

class Walk {
		Location a, b;
		uint32_t dist;
		uint32_t nWalks = 1;

public:
		Walk(Location a, Location b, uint32_t dist);
		Walk(int32_t xa, int32_t ya, int32_t xb, int32_t yb, uint32_t dist);

		void addWalk(uint32_t newdist);
		void setDist(uint32_t dist);


};


#endif //SERVER_WALK_HH
