#include "savings.h"

#include <algorithm> 
#include "vector"

#include "auxiliares.h"
#include "truck.h"

using namespace	std;
#define ninguno -1
namespace savings {
	int n;
	int capacidad;
	float saving_total;
	vector<Truck> solveCvrp(Point& warehouse,	vector<Point> &points, int capacity, Params&){
		/* Armo matriz de distancias entre cada par de nodos */
		n = points.size();
		capacidad = capacity;
		saving_total = 0;
		float distancia_total = 0;
		vector<vector<float> > distancias(n, vector<float>(n, 0));
		/* Vector de distancia de todo nodo a deposito, vector con info de en que camion va cada nodo */
		vector<float> distancia_a_deposito(n, 0);
		vector<int> en_que_camion(n, ninguno);
		/* Vector de savings de tamano Nc2 */
		vector<Saving> savings;
		savings.reserve(n*(n-1)/2);

		/* Inicializo distancias y calculo savings */
		calcularDistancias(distancias, distancia_a_deposito, points, warehouse, distancia_total);
		calcularSavings(distancias, distancia_a_deposito, points, savings);

		/* Ordeno los savings de menor a mayor */
		sort(savings.begin(), savings.end());

		vector<Truck> trucks;
		/* Recorro savings de mayor a menor*/
		for (int i = savings.size()-1; i >= 0 && savings[i].saving > 0; i--){
			Saving &s = savings[i];
			int camion_A = en_que_camion[s.point_A];
			int camion_B = en_que_camion[s.point_B];
			int demanda;

			if (camion_B == ninguno and camion_A == ninguno){
				/* Nuevo camion que vaya a ambos nodos */
				demanda = points[s.point_A].demand + points[s.point_B].demand;
				if (capacidad >= demanda){
					camionNuevo(trucks, en_que_camion, s.point_A, s.point_B, demanda);
					saving_total += s.saving; 
				}
			}

			if (camion_B == ninguno and camion_A != ninguno){
				/* Va al camion de A*/
				demanda = points[s.point_B].demand;
				Truck &t = trucks[camion_A];
				if (puedoAgregarlo(t, s.point_A, points[s.point_B].demand)){
					visitarCliente(t, en_que_camion, s.point_A, s.point_B, demanda);
					saving_total += s.saving; 
				}
			}
			
			if (camion_A == ninguno and camion_B != ninguno){
				/* Va al camion de B*/
				demanda = points[s.point_A].demand;
				Truck &t = trucks[camion_B];
				if (puedoAgregarlo(t, s.point_B, demanda)){
					visitarCliente(t, en_que_camion, s.point_B, s.point_A, demanda);
					saving_total += s.saving; 
				}
			}

			if (camion_B != ninguno and camion_A != ninguno and camion_A != camion_B){
				/* Puedo mergear rutas*/
				if (puedoUnirRutas(trucks[camion_A], trucks[camion_B], s.point_A, s.point_B)){
					unirRutas(trucks, en_que_camion, s.point_A, s.point_B);
					saving_total += s.saving;
				}
				
			}
		}
		for(unsigned int i = trucks.size() - 1; i >= 0 and i < trucks.size(); i--){
			if(!trucks[i].es_valido)
				trucks.erase(trucks.begin() + i);
		}
		for (unsigned int i = 0; i < en_que_camion.size(); i++){
			if (en_que_camion[i] == ninguno){
				trucks.push_back(Truck(capacity, n, i, points[i].demand));
			}
		}
		armarCamiones(trucks, points, distancias, distancia_a_deposito);
		return trucks;
	}

	void calcularDistancias(vector<vector<float> > &distances, vector<float> &distance_to_warehouse, vector<Point> &points, Point& warehouse, float &distancia_total){
		for (unsigned int i = 0; i < points.size(); i++){
			for (unsigned int j = i+1; j < points.size(); j++){
				float point_distance = points[i].DistanceTo(points[j]);
				distances[i][j] = point_distance;
				distances[j][i] = point_distance;
			}
		}
		for (unsigned int i = 0; i < points.size(); i++){
			distance_to_warehouse[i] = points[i].DistanceTo(warehouse);
			distancia_total += distance_to_warehouse[i];
		}
		distancia_total*=2;
	}

	void calcularSavings(vector<vector<float> > &distances, vector<float> &distance_to_warehouse, vector<Point> &points, vector<Saving> &savings){
		for(unsigned int i = 0; i < points.size(); i++){
			for (unsigned int j = i+1; j < points.size(); j++){
				savings.push_back(Saving(distances, distance_to_warehouse, i, j));
			}
		}
	}
	float imprimirCamion(Truck &t, vector<Point> &points, Point& warehouse, vector<vector<float> > &distancias, vector<float> &distance_to_warehouse){
		float distancia_recorrida = 0;
		if (t.es_valido){
			print("Quedó con " << t.stock_left << " de capacidad")
			vector<Point> res;
			res.push_back(warehouse);
			int cliente = t.cliente_final;
			distancia_recorrida += distance_to_warehouse[cliente];
			res.push_back(points[cliente]);
			while(t.predecesores[cliente] != ninguno){
				distancia_recorrida += distancias[cliente][t.predecesores[cliente]];
				cliente = t.predecesores[cliente];
				res.push_back(points[cliente]);
			}
			distancia_recorrida += distance_to_warehouse[cliente];
			res.push_back(warehouse);
			aux::print_vector(res);
			print("----")
		}
		return distancia_recorrida;
	}

	float armarRuta(Truck &t, vector<Point> &points, vector<vector<float> > &distancias, vector<float> &distance_to_warehouse){
		int cliente = t.cliente_final;
		float distancia_recorrida = distance_to_warehouse[cliente];
		t.routes.push_back(points[cliente]);
		while(t.predecesores[cliente] != ninguno){
			distancia_recorrida += distancias[cliente][t.predecesores[cliente]];
			cliente = t.predecesores[cliente];
			t.routes.push_back(points[cliente]);
		}
		distancia_recorrida += distance_to_warehouse[cliente];
		return distancia_recorrida;
	}

	void armarCamiones(vector<Truck> &trucks, vector<Point> &points, vector<vector<float> > &distancias, vector<float> &distance_to_warehouse){
		float distancia_recorrida = 0;
		for(Truck &t : trucks){
			if (t.es_valido){
				distancia_recorrida += armarRuta(t, points, distancias, distance_to_warehouse);	
			}
		}

		//print("Distancia recorrida es " << distancia_recorrida)
	}


	bool puedoAgregarlo(Truck &t, int punto, int demanda){
		return t.noEsInterno(punto) and t.hayEspacio(demanda);
	}

	void camionNuevo(vector<Truck> &trucks, vector<int> &en_que_camion, int punto_A, int punto_B, int demanda){
		trucks.push_back(Truck(capacidad, n, punto_A, punto_B, demanda));
		en_que_camion[punto_A] = trucks.size()-1;
		en_que_camion[punto_B] = trucks.size()-1;
	}

	void visitarCliente(Truck &t, vector<int> &en_que_camion, int existente, int nuevo, int demanda){
		t.visit(existente, nuevo, demanda);
		en_que_camion[nuevo] = en_que_camion[existente];
	}

	bool puedoUnirRutas(Truck &truck_A, Truck &truck_B, int punto_A, int punto_B){
		int capacidad_necesaria = (2*capacidad) - truck_A.stock_left - truck_B.stock_left;
		return truck_A.noEsInterno(punto_A) and truck_B.noEsInterno(punto_B) and capacidad_necesaria <= capacidad;
	}

	void unirRutas(vector<Truck> &trucks, vector<int> &en_que_camion, int punto_A, int punto_B){
		Truck &truck_A = trucks[en_que_camion[punto_A]];
		Truck &truck_B = trucks[en_que_camion[punto_B]];
		int capacidad_necesaria = (2 * capacidad) - truck_A.stock_left - truck_B.stock_left;
		truck_A.mergearRuta(punto_A, punto_B, truck_B, capacidad - capacidad_necesaria, en_que_camion);
	}

}
