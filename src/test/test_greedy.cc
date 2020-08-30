#include "test_greedy.h"

void RunAllTestsForGreedy(){
	TestGetDemandsRange();
	TestBucketSort();
	TestSortEachBucketByClosenessTo();
	TestFindFittestBucket1();
	TestFindFittestBucket2();
	TestPopNextVertex1();
	TestPopNextVertex2();
}

void TestGetDemandsRange(){
	TEST

	given("A list of points with their respective demands")
		vector<Point> points = {Point(X, Y, 1),
								Point(X, Y, 2),
								Point(X, Y, 3),
								Point(X, Y, 4),
								Point(X, Y, 5)};

	when("GetDemandsRange is called")
		pair<int, int> range = greedy::GetDemandsRange(points);

	then("1 should be minimum and 5 should be maximum")
		assert(range.first == 1);
		assert(range.second == 5);

	END_TEST
}

void TestBucketSort(){
	TEST

	given("A list of points with their respective demands")
		vector<Point> points = {Point(X, Y, 2),
								Point(X, Y, 5),
								Point(X, Y, 1),
								Point(X, Y, 2),
								Point(X, Y, 1)};
		int MIN_DEMAND = 1;
		int MAX_DEMAND = 5;

	when("BucketSort is called")
		Buckets buckets = greedy::BucketSort(points, MAX_DEMAND);

	then("Should be correctly sorted")
		Buckets expected_buckets(6);
		expected_buckets[1].push_back(Point(X, Y, 1));
		expected_buckets[1].push_back(Point(X, Y, 1));
		expected_buckets[2].push_back(Point(X, Y, 2));
		expected_buckets[2].push_back(Point(X, Y, 2));
		expected_buckets[5].push_back(Point(X, Y, 5));

		assert(buckets == expected_buckets);

	END_TEST
}

void TestSortEachBucketByClosenessTo(){
	TEST

	given("Some buckets and a warehouse point")
		Buckets buckets(6);
		buckets[1].push_back(Point(3, 1, 1));
		buckets[1].push_back(Point(4, 1, 1));
		buckets[1].push_back(Point(2, 1, 1));
		buckets[2].push_back(Point(3, 3, 2));
		buckets[2].push_back(Point(4, 4, 2));
		buckets[5].push_back(Point(2, 2, 5));
		Point warehouse(1, 2, WHATEVER_DEMAND);

	when("SortEachBucketByClosenessTo is called")
		greedy::SortEachBucketByClosenessTo(buckets, warehouse);

	then("Should be correctly sorted in descending order")
		Buckets expected_buckets(6);
		expected_buckets[1].push_back(Point(4, 1, 1));
		expected_buckets[1].push_back(Point(3, 1, 1));
		expected_buckets[1].push_back(Point(2, 1, 1));
		expected_buckets[2].push_back(Point(4, 4, 2));
		expected_buckets[2].push_back(Point(3, 3, 2));
		expected_buckets[5].push_back(Point(2, 2, 5));

		assert(buckets == expected_buckets);

	END_TEST
}

void TestFindFittestBucket1(){
	TEST

	given("One truck with no space available and buckets with vertex too big")
		Buckets buckets(6);
		buckets[2].push_back(Point(3, 3, 2));
		buckets[2].push_back(Point(4, 4, 2));
		buckets[5].push_back(Point(2, 2, 5));

		int truck_total_capacity = 20;
		int last_truck_stock_left = 1;

		vector<Truck> trucks;
		trucks.push_back(Truck(WHATEVER_WAREHOUSE, last_truck_stock_left));
		unsigned int original_trucks_count = trucks.size();

	when("FindFittestBucket is called")
		Bucket* fittestBucket = greedy::FindFittestBucket(buckets, trucks,
			WHATEVER_WAREHOUSE, truck_total_capacity);

	then("I should have a bucket of maximum demands and one more truck on the trucks list")
		Point some_vertex_in_bucket = (*fittestBucket)[0];
		assert(some_vertex_in_bucket.demand == 5);
		assert(trucks.size() == original_trucks_count + 1);
		assert(fittestBucket->size() == 1);

	END_TEST
}

void TestFindFittestBucket2(){
	TEST

	given("One truck with space available and buckets with a vertex not too big")
		Buckets buckets(6);
		buckets[2].push_back(Point(3, 3, 2));
		buckets[2].push_back(Point(4, 4, 2));
		buckets[2].push_back(Point(5, 1, 2));
		buckets[2].push_back(Point(3, 2, 2));
		buckets[5].push_back(Point(2, 2, 5));

		int truck_total_capacity = 20;
		int last_truck_stock_left = 2;

		vector<Truck> trucks;
		trucks.push_back(Truck(WHATEVER_WAREHOUSE, last_truck_stock_left));
		unsigned int original_trucks_count = trucks.size();

	when("FindFittestBucket is called")
		Bucket* fittestBucket = greedy::FindFittestBucket(buckets, trucks,
			WHATEVER_WAREHOUSE, truck_total_capacity);

	then("I should have a bucket of maximum demands and one more truck on the trucks list")
		Bucket expected_bucket = {Point(3, 3, 2), Point(4, 4, 2), Point(5, 1, 2), Point(3, 2, 2)};
		Point some_vertex_in_bucket = (*fittestBucket)[0];
		assert(trucks.size() == original_trucks_count);
		assert(some_vertex_in_bucket.demand == 2);
		assert(expected_bucket == (*fittestBucket));

	END_TEST
}

void TestPopNextVertex1(){
	TEST

	given("A bucket list sorted in euclidean distance to warehouse in decreasing order")
		Point expected_next_vertex = Point(6, 1, 1);
		Bucket bucket = {Point(7, 1, 1),
						 expected_next_vertex,
						 Point(5, 1, 1),
						 Point(4, 1, 1)}; // el último punto es el más cercano al warehouse
		unsigned int original_bucket_size = bucket.size();

		Point current_vertex = Point(6, 2, 1);
		vector<Truck> trucks;
		Truck truck(WHATEVER_WAREHOUSE, WHATEVER_CAPACITY);
		truck.visit(current_vertex);
		trucks.push_back(truck);

	when("PopNextVertex is called")
		Point next_vertex = greedy::PopNextVertex(&bucket, trucks, K);

	then("I should expect to get the vertex (6, 1) that is the closest to my current vertex (6, 2)")
		assert(expected_next_vertex == next_vertex);
		assert(bucket.size() == original_bucket_size - 1);

	END_TEST
}

void TestPopNextVertex2(){
	TEST

	given("A bucket list sorted in euclidean distance to warehouse in decreasing order")

	when("PopNextVertex is called")

	then("I should expect to get the vertex (6, 1) that is the closest to my current vertex (6, 2)")

	END_TEST
}
