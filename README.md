# Vehicle routing algorithms.
The following project has five heuristics for the Capacited Vehicle Routing Problem (CVRP). The problem involves the design of minimum cost delivery routes for a fleet of vehicles, originating and terminating at a central depot which serves a bunch of customers. Each customer is served exactly one time and the total demand of any vehicle must not exceed the vehicle capacity. 
1) Clark-Wright's Savings method. An iterative procedure. 
2) A Greedy constructive heuristic.
3) Cluster-first, route-second. Clustering with Sweeping-algorithm.  
4) Cluster-first, route-second. Clustering with Kmeans-algorithm. 
5) Simmulated Annealing. 

## How to run
cd src/
make clean && make
cd build/
./build/main algorithm resources/file
where algorithm is: savings, greedy, sweep, kmeans or annealing
and file is an input from resources folder. 
