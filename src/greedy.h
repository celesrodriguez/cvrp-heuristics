#ifndef GREEDY_H_
#define GREEDY_H_

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

typedef vector<Point> Bucket;
typedef vector<Bucket> Buckets;

namespace greedy {

	vector<Truck> solveCvrp(Point& warehouse, vector<Point> &points, int capacity, Params& params);

	pair<int, int> GetDemandsRange(vector<Point> &points);

	Buckets BucketSort(vector<Point> &points, int max_demand_value);

	void SortEachBucketByClosenessTo(Buckets& buckets, Point& point);

	Bucket* FindFittestBucket(Buckets &buckets, vector<Truck> &trucks, Point warehouse, int capacity);

	Point PopNextVertex(Bucket* bucket, vector<Truck> &trucks, int k);
}

#endif
