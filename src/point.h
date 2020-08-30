#ifndef POINT_H_
#define POINT_H_

#include <math.h>
#include <iostream>
#include <string>

struct Point {
    int x;
    int y;
    int demand;
    float radius;
    float angle;

    Point(int x, int y, int demand): x(x), y(y), demand(demand){}

    Point(int x, int y, int demand, float radius, float angle): x(x), y(y),
    	demand(demand), radius(radius), angle(angle){}

	float DistanceTo(Point p) const{
		return sqrt(pow(p.x - x, 2) + pow(p.y - y, 2));
	}

	bool operator==(const Point& p) const {
		return x == p.x and y == p.y and demand == p.demand;
	}

	bool operator!=(const Point& p) const {
		return not( this->Point::operator==(p));
	}

	bool operator<(const Point& other) const {
		return demand < other.demand;
	}

	std::string to_string(){
		return std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(demand);
	}
};

#endif
