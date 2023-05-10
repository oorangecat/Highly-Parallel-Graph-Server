//
// Created by marco on 10/05/2023.
//

#ifndef SERVER_LOCATION_HH
#define SERVER_LOCATION_HH

#include <boost/geometry.hpp>
#include <vector>

namespace boostg = boost::geometry;

//point as a pair of int32
using point = boostg::model::point<int32_t, 2, boostg::cs::cartesian>;

class Location {
		point p;
public:

		Location();
		Location(int32_t x, int32_t y) : p(x, y) {}

		point get_p();
		void set_p(point newp);
		void set_p(int32_t x, int32_t y);

		int32_t distance(point second);
		int32_t distance(int32_t x, int32_t y);
		int32_t distance(Location *other);


};


#endif //SERVER_LOCATION_HH
