#include "kmeans.h"

namespace kmeans {
	#define ninguno -1
	#define Punto_Invalido Point(0,0,-1)
	unsigned int n = 0;
	unsigned int k = 0;
	vector<Truck> solveCvrp(Point& warehouse, vector<Point> &points, int max_stock, Params&){
		n = points.size();
		k = calculateK(points, max_stock);
		vector<int> in_cluster(n, ninguno);
		vector<Point> k_clusters;

		initializeClusters(points, k_clusters, in_cluster, max_stock);
		bool sigueCambiando = true;
		int it = 0;
		while (sigueCambiando and it < 100){
			sigueCambiando = false;
			/* Actualiza asignaciones de cada punto a cada cluster */
			for(unsigned int i = 0; i < n; i++){
				int last_cluster = in_cluster[i]; 
				int cluster = findNearestCentroid(k_clusters, points[i], last_cluster);
				/* Si cambio de cluster... */
				if (last_cluster != cluster){
					sigueCambiando = true;
					in_cluster[i] = cluster;
					if (last_cluster != ninguno) k_clusters[last_cluster].demand += points[i].demand;
					k_clusters[cluster].demand -= points[i].demand;
				}			
			}
			/* Actualiza centroids */
			for (unsigned int i = 0; i < k and sigueCambiando; i++){
				Point new_centroid = calculateCentroid(in_cluster, k_clusters[i].demand, i, points);
				/* Si el centro cambio... */
				if (new_centroid != k_clusters[i]){
					k_clusters[i] = new_centroid;
				}
			}
			it++;
		}

		Clusters clusters = BuildClusters(points, in_cluster);
		/* Borro los que quedaron vacios */
		for(unsigned int i = clusters.size() - 1; i > 0; i--)
			if(clusters[i].size() == 0)
				clusters.erase(clusters.begin() + i);
		
		vector<Truck> trucks = sweep::BuildRoutesFromClusters(clusters, warehouse, max_stock);

		return trucks;
	}

	int calculateK(vector<Point> &points, int max_stock){
		/* K es un aproximado de la cantidad de camiones que se necesitan para cubrir las demandas */
		double new_k = 0;
		for (unsigned int i = 0; i < n; i++)
			new_k += points[i].demand;
		return ceil(new_k/max_stock);
	}

	void initializeClusters(vector<Point> &points, vector<Point> &k_clusters, vector<int> &in_cluster, int max_stock){
		/* Agrega los primeros k puntos a los k clusters */
		for (unsigned int i = 0; i < k and i < n; i++){
			k_clusters.push_back(Point(points[i].x, points[i].y, max_stock - points[i].demand));
			in_cluster[i] = k_clusters.size()-1;
		}
	}

	int findNearestCentroid(vector<Point> &k_clusters, Point &point, int cluster){
		vector<Point> clusters_copy(k_clusters);
		Point old_cluster = k_clusters[0];
		if (cluster != ninguno) old_cluster = k_clusters[cluster];
		/* Ordena los centroids por menor distancia al punto */
		sort(clusters_copy.begin(), clusters_copy.end(), DistanceToPoint(point));
		unsigned int i;
		bool termine = false;
		for (i = 0; i < clusters_copy.size() and !termine; i++){
			if (cluster == ninguno or clusters_copy[i] != old_cluster){
				/* Si no es el mismo cluster al que pertenecia el punto*/
				int capacity_left = clusters_copy[i].demand;
				/* Si hay suficiente espacio */
				if (capacity_left >= point.demand) termine = true;
			}else{
				/* Queda en el mismo */
				termine = true;
			}
		}
		/* i-1 es indice si termine */
		if (!termine){
			/* Nuevo cluster */
			k_clusters.push_back(point);
			k++;
			return k-1; // con indice k-1
		} 
		/* Devuelve el nuevo cluster con indice segun la entrada */
		return aux::GetPointId(k_clusters, clusters_copy[i-1])-1;
	}

	Point calculateCentroid(vector<int> &in_cluster, int demand, int cluster, vector<Point> &points){
		/* Calcula el punto promedio entre los puntos del cluster, si los hay */
		float mean_x = 0;
		float mean_y = 0;
		int cant = 0;
		for (unsigned int i = 0; i < n; i++){
			if (in_cluster[i] == cluster){
				mean_x += points[i].x;
				mean_y += points[i].y;
				cant++;
			}
		}
		/* Si el cluster era vacio */
		if (cant == 0) return Punto_Invalido;
		mean_x /= cant;
		mean_y /= cant;

		return Point(floor(mean_x), floor(mean_y), demand);
	}

	Clusters BuildClusters(vector<Point> &points, vector<int> &in_cluster){
		/* Agrega los puntos a su cluster, todos estan en exactamente uno */
		Clusters clusters(k, Cluster());
		for(unsigned int i = 0; i < n; i++){
			clusters[in_cluster[i]].push_back(points[i]);
		}

		return clusters;
	}
	
}