#ifndef AUXILIARES_H_
#define AUXILIARES_H_

#define DEBUG true
#define PRINT false
#define print(x) if( PRINT ) std::cout << x << "\n";
#define debug(x) if( DEBUG ) std::cout << x << "\n";

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cctype>

#include "edge.h"
#include "point.h"
#include "truck.h"

namespace aux {
	std::ostream& operator<<(std::ostream &strm, const Point &p);

	float mod(float a, float mod);

	int min(int a, int b);

	int max(int a, int b);

	template <typename T>
	void print_vector(const std::vector<T> &S, std::ostream &output, std::string separator){
		for(unsigned int i = 0; i < S.size(); i++) {
			output << S[i];
			if (i != S.size() - 1) {
				output << separator;
			}
		}
	}

	template <typename T>
	void print_vector(const std::vector<T> &S){
		std::cout << "\n{ ";
		print_vector(S, std::cout, ", ");
		std::cout << " }";
	}

	void print_edges(const std::vector<Edge> &S);

	void print_buckets(const std::vector<std::vector<Point>>& buckets);

	int GetPointId(vector<Point> &points, Point &point);

	void PrintTrucks(vector<Point> &points, Point &warehouse, vector<Truck> &trucks);

	void removeDeposits(vector<Truck> &trucks);

	bool to_bool(std::string str);

	std::vector<Truck> parseTrucks(int capacity, std::vector<std::vector<int>> truck_list, std::vector<Point> &points);
}

#endif
