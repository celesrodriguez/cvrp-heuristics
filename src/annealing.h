#ifndef ANNEALING_H_
#define ANNEALING_H_

#include <vector>
#include <tuple>
#include <random>

#include "point.h"
#include "truck.h"
#include "params.h"


namespace annealing {

	std::vector<Truck> solveCvrp(Point& warehouse, vector<Point> &points, int capacity, Params &params);

	std::tuple<float, float> getTemperatureRange(const Point &warehouse, std::vector<Truck> &trucks);

	float calculateSolutionEnergy(const std::vector<Truck> &solution, const Point &warehouse);

	bool shouldAccepSolution(float cost, float current_temp, std::default_random_engine &rand_generator);

	float coolDown(float starting_temp, float final_temp, float current_temp, int n, int iterations, int alpha);

	float heatUp(float reset_temp, float best_temp);

	void printSolution(int iterations, float current_energy, float current_temp, std::vector<Point> &points, Point &warehouse, std::vector<Truck> &current_solution);
}

#endif
