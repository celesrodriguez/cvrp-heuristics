#include "annealing.h"

#include <vector>
#include <tuple>
#include <math.h>
#include <cmath>
#include <random>
#include <cstdlib>
#include <ctime>

#include "greedy.h"
#include "neighborhood.h"
#include "auxiliares.h"

#define PRINT_SOLUTION printSolution(iterations, current_energy, current_temp, points, warehouse, current_solution);
#define EXCHANGES 5000
#define RESETS 3
#define EPSILON 0.001

namespace annealing {

	std::vector<Truck> solveCvrp(Point& warehouse, vector<Point> &points, int capacity, Params &params){
		int iterations = 0, n = points.size();
		std::vector<Truck> current_solution, best_solution;
		float current_energy, best_energy;

		current_solution = greedy::solveCvrp(warehouse, points, capacity, params);
		current_energy = calculateSolutionEnergy(current_solution, warehouse);
		best_solution = current_solution; // Copy
		best_energy = current_energy; 
		float starting_temp, final_temp, current_temp, best_temp, reset_temp;
		float alpha, gamma;

		auto temperature_range = getTemperatureRange(warehouse, current_solution);
		final_temp = get<0>(temperature_range); // Min change in cost
		starting_temp = get<1>(temperature_range); // Max change in cost
		current_temp = starting_temp;
		best_temp = current_temp;
		reset_temp = current_temp;

		Neighborhood neighborhood(warehouse, current_solution); // Neighborhood holds a reference to solution in order to modify it.

		std::default_random_engine rand_generator;
		std::srand( time(NULL) );

		int current_resets = 0;

		PRINT_SOLUTION

		while (current_resets < RESETS) {
			iterations++;
			if (neighborhood.HasNeighborsLeft()) {
				float cost = neighborhood.NextNeighbor();
				if ( shouldAccepSolution(cost, current_temp, rand_generator) ) {
					neighborhood.AcceptNeighbor();
					current_energy += cost;
					PRINT_SOLUTION
				}
				if (current_energy < best_energy) {
					best_energy = current_energy;
					best_solution = current_solution; // Copy
					best_temp = current_temp;
					current_resets = 0;
				}
				current_temp = coolDown(starting_temp, final_temp, current_temp, n, iterations, n * EXCHANGES);
			} else {
			        current_temp = heatUp(reset_temp, best_temp); 
				reset_temp = current_temp;
				current_resets++;
				neighborhood.CreateNeighborhood();
			}
		}

		return best_solution;
	}

	std::tuple<float, float> getTemperatureRange(const Point &warehouse, std::vector<Truck> &solution){
		Neighborhood neighborhood(warehouse, solution);
		float min, max;
		min = std::abs(neighborhood.NextNeighbor());
		max = min;
		while (neighborhood.HasNeighborsLeft()) {
			float cost_change = std::abs(neighborhood.NextNeighbor());
			if (cost_change < min)
				min = cost_change;
			else if (cost_change > max)
				max = cost_change;
		}
		return std::make_tuple(min + EPSILON, max);
	}

	float calculateSolutionEnergy(const std::vector<Truck> &solution, const Point &warehouse){
		float energy = 0;
		for (const Truck &truck : solution)
			energy += truck.distanceTravelled(warehouse);
		return energy;
	}

	bool shouldAccepSolution(float cost, float current_temp, std::default_random_engine &rand_generator) {
		std::uniform_real_distribution<float> distribution(0.0,1.0);
		float accept_probability = distribution(rand_generator);
		return cost < 0 or std::exp(-(cost + EPSILON) / current_temp) >= accept_probability;
	}

	float coolDown(float starting_temp, float final_temp, float current_temp, int n, int iterations, int alpha) {
		float beta = (starting_temp - final_temp) / ( (alpha + n * std::sqrt(iterations) ) * starting_temp * final_temp);
		return current_temp / (1 + beta * current_temp);
	}
	
	float heatUp(float reset_temp, float best_temp) {
		return reset_temp / 2 > best_temp ? reset_temp/2 : best_temp;
	}

	void printSolution(int iterations, float current_energy, float current_temp, std::vector<Point> &points, Point &warehouse, std::vector<Truck> &current_solution){
		if (PRINT) { 
			std::cout << "---" << endl;
			std::cout << iterations << endl;
			std::cout << current_energy << endl;
			std::cout << current_temp << endl;
			aux::PrintTrucks(points, warehouse, current_solution);
		}
	}
}
