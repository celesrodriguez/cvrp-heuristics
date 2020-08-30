#include "iostream"
#include "test_greedy.h"
#include "test_sweep.h"

int main (){
	std::cout << "\nRunning greedy tests\n";
	std::cout << "================================\n";
	RunAllTestsForGreedy();

	std::cout << "\nRunning sweep tests\n";
	std::cout << "================================\n";
	RunAllTestsForSweep();

	return 0;
}
