#include "sweep.h"

namespace sweep {
	vector<Truck> solveCvrp(Point& warehouse, vector<Point> &p, int max_stock, Params &params){
		vector<Point> points(p);

		TransformPointsFromCartesianToPolar(warehouse, points);
		sort(points.begin(), points.end(), AngleComparator());

		if(params.opt1_as_bool()){ // adaptative sweeping
			float sweep_starting_angle = FindSweepStartingAngle(points);
			sort(points.begin(), points.end(), AngleComparator(sweep_starting_angle));
		}

		Clusters clusters = BuildClusters(points, max_stock);
		vector<Truck> routes = BuildRoutesFromClusters(clusters, warehouse, max_stock);

		aux::removeDeposits(routes); // es extra-algoritmico (no lo contamos en la complejidad)
		return routes;
	}

	void TransformPointsFromCartesianToPolar(Point &warehouse, vector<Point> &points){
		int relative_x, relative_y;

		for(Point &p : points){
			relative_x = p.x - warehouse.x;
			relative_y = p.y - warehouse.y;

			p.radius = sqrt(pow(relative_x, 2) + pow(relative_y, 2));
			if (relative_x > 0) p.angle = atan2(relative_y, p.radius + relative_x);
			if (relative_x <= 0 and relative_y!=0) p.angle = atan2(p.radius - relative_x, relative_y);
			if (relative_x < 0 and relative_y==0) p.angle = PI;
			if (p.angle < 0) p.angle += 2*PI;
		}
	}

	float FindSweepStartingAngle(vector<Point> &points){
		Point prev = points[0];
		float greatest_angle_gap = 0;
		float ray_angle = 0;

		for(unsigned int i = 1; i <= points.size(); i++){
			unsigned int index = i % points.size();
			Point current = points[index];

			float gap = 0;
			if(prev.angle < current.angle)
				gap = fabs(prev.angle - current.angle);
			else
				gap = (2 * PI - prev.angle + current.angle);

			if(gap > greatest_angle_gap){
				greatest_angle_gap = gap;
				ray_angle = aux::mod((prev.angle + gap / 2), 2*PI);
			}

			prev = current;
		}

		return ray_angle;
	}

	Clusters BuildClusters(vector<Point> &points, int max_stock){
		Clusters clusters;
		clusters.push_back(Cluster());
		int current_stock = max_stock;

		for(Point &p : points){
			if(current_stock < p.demand){
				current_stock = max_stock;
				clusters.push_back(Cluster());
			}

			current_stock -= p.demand;
			clusters.back().push_back(p);
		}

		return clusters;
	}

	vector<Truck> BuildRoutesFromClusters(Clusters &clusters, Point &warehouse, int max_stock){
		vector<Truck> trucks = { Truck(warehouse, max_stock) };

		for(unsigned int i = 0; i < clusters.size(); i++){
			Cluster &cluster = clusters[i];

			while(not cluster.empty()){
				Truck &last_truck = trucks.back();
				Point *last_visited;

				if(last_truck.empty()){
					last_visited = &(warehouse);
				}else{
					last_visited = &(last_truck.LastVisited());
				}

				last_truck.visit(PopClosestVertexTo(cluster, (*last_visited)));
			}

			trucks.back().visit(warehouse);

			if(i < clusters.size() - 1)
				trucks.push_back(Truck(warehouse, max_stock));

		}

		return trucks;
	}

	Point PopClosestVertexTo(Cluster &cluster, Point &point){
		sort(cluster.begin(), cluster.end(), DistanceToPointComparator(point));
		Point closest = cluster.back();
		cluster.erase(cluster.end() - 1);

		return closest;
	}
}
