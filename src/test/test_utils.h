#ifndef TEST_UTILS_H_
#define TEST_UTILS_H_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define TEST printf("%s... ", __func__);
#define END_TEST printf("PASSED\n");
#define IGNORE if(0) 
#define given(x)
#define when(x)
#define then(x)
#define And(x)
#define Case(x)
#define cleanup

#define X 0
#define Y 0
#define EPSILON 0.00001

#define WHATEVER_DEMAND 2
#define WHATEVER_CAPACITY 2
#define WHATEVER_WAREHOUSE Point(0, 0, 0)

bool fequals(float a, float b);

#endif
