#ifndef TEST_SWEEP_H_
#define TEST_SWEEP_H_

#include <utility>
#include "test_utils.h"
#include "../sweep.h"

typedef vector<Point> Cluster;
typedef vector<Cluster> Clusters;

void RunAllTestsForSweep();
void TestTransformPointsFromCartesianToPolar();
void TestBuildClusters();
void TestBuildRoutesFromClusters();
void TestPopClosestVertexTo();
void TestFindSweepStartingAngle1();
void TestFindSweepStartingAngle2();

#endif
