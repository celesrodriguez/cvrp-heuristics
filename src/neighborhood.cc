#include "neighborhood.h"

#include <stdexcept>
#include <algorithm>
#include <tuple>
#include <iterator>
#include <limits>

#include "auxiliares.h"

// ===== Public  ====
Neighborhood::Neighborhood(const Point &warehouse, std::vector<Truck> &trucks) : warehouse(warehouse), trucks(trucks) {
	CreateNeighborhood();
}

float Neighborhood::NextNeighbor(){

	if (not HasNeighborsLeft()) 
		throw std::logic_error("No more neighbors to explore");
	
	if (std::next(current_interchange, 1) == interchanges.end()) {
		do {
			std::advance(current_trucks, 1);
			if( current_trucks == truckCombinations.end() ) 
				return std::numeric_limits<float>::max(); // We're all out of trucks, the neighborhood reached a stalemate
			interchanges = GenerateInterchanges(trucks[get<0>(*current_trucks)], trucks[get<1>(*current_trucks)]);
			current_interchange = interchanges.begin();
		} while (current_interchange == interchanges.end()); // Two routes may produce no interchanges on odd occasions
	} else {
		std::advance(current_interchange, 1);
	}

	float cost = 0;
	Truck &left_truck = trucks[get<0>(*current_trucks)];
	Truck &right_truck = trucks[get<1>(*current_trucks)];
	switch(current_interchange->operation){
		case shift_left: {
			// Takes one client from the right side and inserts it into the left side
			float right_deletion_cost = CalculateDeletionCost(right_truck, current_interchange->right);
			float left_insertion_cost = CalculateInsertionCost(left_truck, right_truck.getClient(current_interchange->right), current_interchange->left);
			cost = left_insertion_cost + right_deletion_cost;
			break;
		} case shift_right: {
			// Takes one client from the left side and inserts it into the right side
			float left_deletion_cost = CalculateDeletionCost(left_truck, current_interchange->left);
			float right_insertion_cost = CalculateInsertionCost(right_truck, left_truck.getClient(current_interchange->left), current_interchange->right);
			cost = right_insertion_cost + left_deletion_cost;
			break;
		} case exchange: {
			// Exchanges two clients
			Point right_client = right_truck.getClient(current_interchange->right);	
			Point left_client = left_truck.getClient(current_interchange->left);	
			float left_replacement_cost = CalculateReplacementCost(left_truck, right_client, current_interchange->left);
			float right_replacement_cost = CalculateReplacementCost(right_truck, left_client, current_interchange->right);
			cost = left_replacement_cost + right_replacement_cost;
			break;
		}
	}

	return cost; 
}

bool Neighborhood::HasNeighborsLeft() const {
	return not ((std::next(current_trucks, 1) == truckCombinations.end() and std::next(current_interchange, 1) == interchanges.end())
		or (current_trucks == truckCombinations.end()));
}

void Neighborhood::AcceptNeighbor(){
	// A proper state of the instance is assumed.
	// NextNeighbor must've been called before calling AcceptNeighbor
	Truck &left_truck = trucks[get<0>(*current_trucks)];
	Truck &right_truck = trucks[get<1>(*current_trucks)];
	
	switch(current_interchange->operation){
		case shift_left: {
			// Takes one client from the right side and inserts it into the left side
			Point new_client = right_truck.removeAt(current_interchange->right);
			left_truck.insertAt(new_client, current_interchange->left);
			break;
		} case shift_right: {
			// Takes one client from the left side and inserts it into the right side
			Point new_client = left_truck.removeAt(current_interchange->left);
			right_truck.insertAt(new_client, current_interchange->right);
			break;
		} case exchange: {
			// Exchanges two clients
			Point left_client = left_truck.removeAt(current_interchange->left);
			Point right_client = right_truck.removeAt(current_interchange->right);
			left_truck.insertAt(right_client, current_interchange->left); 
			right_truck.insertAt(left_client, current_interchange->right);
			break;
		}
	}

	CreateNeighborhood();
}

void Neighborhood::CreateNeighborhood(){
	RemoveEmptyTrucks();
	truckCombinations = GetRouteCombinations(trucks);
	current_trucks = truckCombinations.begin();
	interchanges = GenerateInterchanges(trucks[get<0>(*current_trucks)], trucks[get<1>(*current_trucks)]);
	// This will be advanced automatically when next neighbor is called
	current_interchange = std::prev(interchanges.begin(), 1); 
}

// ===== Private ====

std::vector<std::tuple<int, int>> Neighborhood::GetRouteCombinations(const std::vector<Truck> &trucks){
	std::vector<std::tuple<int, int>> routeCombinations;
	for(unsigned int i = 0; i < trucks.size() - 1; i++)
		for( unsigned int j = i + 1; j < trucks.size(); j++ )
			routeCombinations.push_back(std::make_tuple(i, j));

	std::random_shuffle(routeCombinations.begin(), routeCombinations.end());
	return routeCombinations;
}

std::vector<Interchange> Neighborhood::GenerateInterchanges(const Truck &Rp, const Truck &Rq){
	std::vector<Interchange> interchanges;
	for(unsigned int left = 0; left < Rp.size(); left++ )
		for(unsigned int right = 0; right < Rq.size(); right++ ){

			int q_demand = Rq.clientDemand(right);
			if( Rp.hayEspacio(q_demand) )
				interchanges.push_back(Interchange(left, right, shift_left));

			int p_demand = Rp.clientDemand(left);
			if( Rq.hayEspacio(p_demand) )
				interchanges.push_back(Interchange(left, right, shift_right));

			bool pHasEnoughStock = q_demand <= p_demand or Rp.hayEspacio(q_demand - p_demand);
			bool qHasEnoughStock = p_demand <= q_demand or Rq.hayEspacio(p_demand - q_demand);
			if( pHasEnoughStock and qHasEnoughStock )
				interchanges.push_back(Interchange(left, right, exchange));
		}

	return interchanges;
}

float Neighborhood::CalculateDeletionCost(const Truck &truck, int index) const {
	Point client = truck.getClient(index);
	Point left = truck.isFirstClient(index) ? warehouse : truck.getClient(index - 1);
	Point right = truck.isLastClient(index) ? warehouse : truck.getClient(index + 1);
	return left.DistanceTo(right) - (left.DistanceTo(client) + client.DistanceTo(right));
}

// insertion cost - siempre se inserta a la izquierda, lola
float Neighborhood::CalculateInsertionCost(const Truck &truck, const Point &client, int index) const {
	Point left = truck.isFirstClient(index) ? warehouse : truck.getClient(index - 1);
	Point right = truck.getClient(index);
	return (left.DistanceTo(client) + client.DistanceTo(right)) - left.DistanceTo(right);
}

float Neighborhood::CalculateReplacementCost(const Truck &truck, const Point &replacement, int index) const {
	Point left = truck.isFirstClient(index) ? warehouse : truck.getClient(index - 1);
	Point right = truck.isLastClient(index) ? warehouse : truck.getClient(index + 1);
	Point replaced = truck.getClient(index);
	return (left.DistanceTo(replacement) + replacement.DistanceTo(right)) - (left.DistanceTo(replaced) + replaced.DistanceTo(right));
}

void Neighborhood::RemoveEmptyTrucks(){
	std::vector<Truck> non_empty_trucks;
	for(Truck &truck : trucks) {
		if (not truck.empty())
			non_empty_trucks.push_back(truck);
	}
	trucks = non_empty_trucks;
}
