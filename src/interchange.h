#ifndef INTERCHANGE_H_
#define INTERCHANGE_H_

enum Operation { shift_left, shift_right, exchange }; // (0,1) (1,0) (1,1) respectively
struct Interchange {
	unsigned int left;
	unsigned int right;
	Operation operation;
	Interchange(unsigned int left, unsigned int right, Operation operation): left(left), right(right), operation(operation){}
};

#endif
