#ifndef KMEANS_H_
#define KMEANS_H_

#include "vector"
#include "point.h"
#include "truck.h"
#include "compare.h"
#include "auxiliares.h"
#include "params.h"
#include <algorithm>
#include "sweep.h"

using namespace std;

typedef vector<Point> Cluster;
typedef vector<Cluster> Clusters;

namespace kmeans {

	vector<Truck> solveCvrp(Point& warehouse, vector<Point> &points, int capacity, Params& params UNUSED);

	int calculateK(vector<Point> &points, int max_stock);

	void initializeClusters(vector<Point> &points, vector<Point> &k_clusters, vector<int> &in_cluster, int max_stock);

	int findNearestCentroid(vector<Point> &k_clusters, Point &p, int cluster);

	Point calculateCentroid(vector<int> &in_cluster, int demand, int cluster, vector<Point> &points);

	Clusters BuildClusters(vector<Point> &points, vector<int> &in_cluster);
}

#endif
