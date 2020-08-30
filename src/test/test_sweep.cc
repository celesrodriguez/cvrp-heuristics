#include "test_sweep.h"

void RunAllTestsForSweep(){
	TestTransformPointsFromCartesianToPolar();
	TestBuildClusters();
	TestBuildRoutesFromClusters();
	TestPopClosestVertexTo();
	TestFindSweepStartingAngle1();
	TestFindSweepStartingAngle2();
}

void TestTransformPointsFromCartesianToPolar(){
	TEST

	given("A list of cartesian points and some warehouse")
		vector<Point> points = {Point(5, 5, WHATEVER_DEMAND), Point(2, 4, WHATEVER_DEMAND)};
		Point warehouse(4, 4, WHATEVER_DEMAND);

	when("Calling TransformPointsFromCartesianToPolar")
		sweep::TransformPointsFromCartesianToPolar(warehouse, points);

	then("The points should have their polar attributes loaded")
		float first_point_expected_angle = M_PI / 4;
		float first_point_expected_radius = sqrt(2);

		float second_point_expected_angle = M_PI;
		float second_point_expected_radius = 2;

		assert(fequals(first_point_expected_angle, points[0].angle));
		assert(fequals(first_point_expected_radius, points[0].radius));
		assert(fequals(second_point_expected_angle, points[1].angle));
		assert(fequals(second_point_expected_radius, points[1].radius));

	END_TEST
}

void TestBuildClusters(){
	TEST

	given("A list of sorted polar points")
		vector<Point> points = {Point(2, 0, 4, 0, 1/4), Point(3, 0, 3, 0, 1/3), 
								Point(4, 0, 6, 0, 3/5), Point(1, 0, 4, 0, 3/4)};
		int max_stock = 10;

	when("Calling BuildClusters")
		Clusters clusters = sweep::BuildClusters(points, max_stock);

	then("Clusters should be built correctly according to capacity")
		Clusters expected_clusters = {{Point(2, 0, 4, 0, 1/4), Point(3, 0, 3, 0, 1/3)}, 
									  {Point(4, 0, 6, 0, 3/5), Point(1, 0, 4, 0, 3/4)}};

		assert(expected_clusters == clusters);

	END_TEST	
}

// to do
void TestBuildRoutesFromClusters(){
	TEST

	given("A list of cartesian points and some warehouse")

	when("Calling TransformPointsFromCartesianToPolar")

	then("The points should have their polar attributes loaded")

	END_TEST
}

void TestPopClosestVertexTo(){
	TEST

	given("A list of cartesian points and a special point")
		Point point_of_interest = Point(2, 5, 1);
		Point closest = Point(2, 4, 3);
		Cluster cluster = {Point(7, 1, 3), Point(4, 2, 3), closest, Point(1, 1, 1)};

	when("Calling TransformPointsFromCartesianToPolar")
		Point result = sweep::PopClosestVertexTo(cluster, point_of_interest);

	then("The points should have their polar attributes loaded")
		assert(closest == result);

	END_TEST
}

void TestFindSweepStartingAngle1(){
	TEST

	given("A list of angles")
		vector<Point> angles = {Point(0, 0, 0, 0, 0.2), Point(1, 0, 0, 0, 0.6), Point(2, 0, 0, 0, 1.7),
								Point(3, 0, 0, 0, 1.9)};

	when("Calling FindMaxAngleSeparation")
		float starting_angle = sweep::FindSweepStartingAngle(angles);

	then("The points should have their polar attributes loaded")
		float expected = 1.15;
		assert(fequals(expected, starting_angle));

	END_TEST
}

void TestFindSweepStartingAngle2(){
	TEST

	given("A list of angles")
		vector<Point> angles = {Point(0, 0, 0, 0, 0.2), Point(1, 0, 0, 0, 0.6), Point(2, 0, 0, 0, 1.0),
								Point(3, 0, 0, 0, 1.4)};

	when("Calling FindMaxAngleSeparation")
		float starting_angle = sweep::FindSweepStartingAngle(angles);

	then("The points should have their polar attributes loaded")
		float expected = 1.8;
		assert(fequals(expected, starting_angle));

	END_TEST
}