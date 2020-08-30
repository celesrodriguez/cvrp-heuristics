#ifndef EDGE_H_
#define EDGE_H_

struct Edge {
	unsigned int v;
	unsigned int u;
	float distance;

	Edge(unsigned int v, unsigned int u, float distance): v(v), u(u), distance(distance){}

	bool operator==(const Edge& e) const {
		return (v == e.v and u == e.u) or (v == e.u and u == e.v);
	}

	bool operator<(const Edge& other) const {
		return distance < other.distance;
	}
};

#endif
