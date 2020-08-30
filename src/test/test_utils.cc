#include "test_utils.h"

bool fequals(float a, float b){
	return fabs(a - b) < EPSILON;
}