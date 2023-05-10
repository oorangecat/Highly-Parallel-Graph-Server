//
// Created by marco on 10/05/2023.
//

#include "Location.hh"

Location::Location(){

}

point Location::get_p(){
	return this->p;
}

void Location::set_p(point newp){
	this->p = newp;
}

void Location::set_p(int32_t x, int32_t y){
	point newp(x,y);
	this->p = newp;
}

int32_t Location::distance(point second){
	return boostg::distance(p, second);
}

int32_t Location::distance(Location *other) {
	return distance(other->p);
}

int32_t Location::distance(int32_t x, int32_t y){
	point newp(x,y);
	return distance(newp);
}