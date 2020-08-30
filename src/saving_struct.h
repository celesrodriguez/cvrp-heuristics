#ifndef SAVINGSTR_H_
#define SAVINGSTR_H_

#include <math.h>
#include <iostream>

struct Saving {
	float saving;
    int point_A;
    int point_B;

    Saving(float saving, int point_A, int point_B): saving(saving), point_A(point_A), point_B(point_B){}

    Saving(std::vector<std::vector<float> > distancias, std::vector<float> distancia_a_deposito, int point_A, int point_B): point_A(point_A), point_B(point_B){
    	saving = distancia_a_deposito[point_A] + distancia_a_deposito[point_B] - distancias[point_A][point_B];
    }

	bool operator==(const Saving& s) const {
		return saving == s.saving and ((point_A == s.point_A and point_B == s.point_B) or (point_A == s.point_B and point_B == s.point_A)) ;
	}

	bool operator<(const Saving& other) const {
		return saving < other.saving;
	}
};


struct saving_it : public std::vector<Saving>::iterator{
	using std::vector<Saving>::iterator::iterator;

	float operator*(){
		return std::vector<Saving>::iterator::operator*().saving;
	}
};

#endif
