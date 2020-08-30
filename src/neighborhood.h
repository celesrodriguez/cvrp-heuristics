#ifndef NEIGHBORHOOD_H_
#define NEIGHBORHOOD_H_

#include <vector>
#include <tuple>

#include "interchange.h"
#include "truck.h"
#include "point.h"

class Neighborhood {

	private:
		const Point warehouse;
		std::vector<Truck> &trucks;
		std::vector<std::tuple<int, int>> truckCombinations;
		std::vector<Interchange> interchanges;
		std::vector<std::tuple<int, int>>::iterator current_trucks;
		std::vector<Interchange>::iterator current_interchange;

		std::vector<std::tuple<int, int>> GetRouteCombinations(const std::vector<Truck> &trucks);
		std::vector<Interchange> GenerateInterchanges(const Truck &Rp, const Truck &Rq);
		float CalculateDeletionCost(const Truck &truck, int index) const ;
		float CalculateInsertionCost(const Truck &truck, const Point &client, int index) const ;
		float CalculateReplacementCost(const Truck &truck, const Point &replacement, int index) const ;
		void RemoveEmptyTrucks();

	public:
		Neighborhood(const Point &warehouse, std::vector<Truck> &trucks);
		
		// Returns the cost of the next neighbor
		float NextNeighbor();
		bool HasNeighborsLeft() const;
		void AcceptNeighbor();
		void CreateNeighborhood();
};

#endif
