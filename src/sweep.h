#ifndef SWEEP_H_
#define SWEEP_H_

#include "vector"
#include "point.h"
#include "truck.h"
#include "compare.h"
#include "auxiliares.h"
#include <utility>
#include <assert.h>
#include <algorithm>
#include "params.h"

using namespace std;

#define PI 3.14159265358979323846

typedef vector<Point> Cluster;
typedef vector<Cluster> Clusters;

namespace sweep {

	vector<Truck> solveCvrp(Point& warehouse, vector<Point> &p, int capacity, Params &params);

	void TransformPointsFromCartesianToPolar(Point &warehouse, vector<Point> &points);

	float FindSweepStartingAngle(vector<Point> &points);

	Clusters BuildClusters(vector<Point> &points, int max_stock);

	vector<Truck> BuildRoutesFromClusters(Clusters &clusters, Point &warehouse, int max_stock);

	Point PopClosestVertexTo(Cluster &cluster, Point &point);

	vector<Truck> removeDeposits(vector<Truck> &trucks);
	
}

#endif
