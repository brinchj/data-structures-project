#include <iostream> // std::cout
#include <fstream>
#include <utility> // std::pair
#include <sstream>
#include <vector>
#include <list>
#include <stdio.h>
#include <ctime>
#include "limits.h"

#include "pennant-node.h++"
#include "stl-meldable-priority-queue.h++"

#include "pairing-heap-framework.h++"
#include "pairing-heap-policy-strict.c++"
#include "pairing-heap-policy-lazy-insert.c++"
#include "pairing-heap-policy-lazy-increase.c++"
#include "pairing-heap-policy-lazy-insert-increase.c++"


using namespace cphstl;

class Vertex;

int compCount;
int incrCount;

class Edge { // {{{
	public:
		Edge(Vertex *from, Vertex *to, int w) : weight(w), from_vertex(from), to_vertex(to) {
		}

		Vertex *getFromVertex() {
			return from_vertex;
		}
		
		Vertex *getToVertex() {
			return to_vertex;
		}

		int getWeight() {
			return weight;
		}
	private:
		int weight;
		Vertex *from_vertex;
		Vertex *to_vertex;
}; // }}}

class Vertex { // {{{
	public:
		Vertex(int vertex_id, std::string vertex_name) : id(vertex_id), name(vertex_name) {
			dist = INT_MAX;
			previous = NULL;
			extracted = false;
		}

		void setDist(int newdist) {
			dist = newdist;
		}

		int getDist() {
			return dist;
		}

		int getId() {
			return id;
		}

		std::string getName() {
			return name;
		}

		bool isExtracted() {
			return extracted;
		}

		void setExtracted(bool isExt) {
			extracted = isExt;
		}

		void setPrev(Vertex *newprev) {
			previous = newprev;
		}

		Vertex *getPrev() {
			return previous;
		}

		void addEdge(Edge *e) {
			adjacent_edges.push_back(e);
		}

		std::vector<Edge*> *getEdges() {
			return &adjacent_edges;
		}

		inline bool
		operator<(const Vertex& __v) const {
			return dist < __v.dist;
		}
		
		inline bool
		operator>(const Vertex& __v) const {
			return dist > __v.dist;
		}
	private:
		int dist;
		int id;
		bool extracted;
		Vertex *previous;
		std::string name;
		std::vector<Edge*> adjacent_edges;
}; // }}}

class vertex_comparator {
public:
  
  bool operator()(Vertex *a, Vertex *b) const {
    ++compCount;
    return (*a) > (*b);
  }
};

typedef Vertex* V;
typedef vertex_comparator C;
typedef std::allocator<V> A;
#if 1
typedef cphstl::pairing_heap_node<V, A, C> E;
//typedef cphstl::pairing_heap_policy_strict<V, C, A, E> POL;
//typedef cphstl::pairing_heap_policy_lazy_insert<V, C, A, E> POL;
//typedef cphstl::pairing_heap_policy_lazy_increase<V, C, A, E> POL;
typedef cphstl::pairing_heap_policy_lazy_insert_increase<V, C, A, E> POL;
typedef cphstl::pairing_heap_framework<V, POL, C, A, E> PQ;
typedef cphstl::meldable_priority_queue<V, C, A, E, PQ> Q;
#endif
#if 0
typedef cphstl::pennant_node<V, A> N;
typedef cphstl::meldable_priority_queue<V, C, A, N> Q;
#endif

#define GRAPHFILE "100k-10m.dat"

std::vector<Q::iterator> vertex_list;

Vertex *get_vertex(std::string name) { // {{{
	for(std::vector<Q::iterator>::iterator it = vertex_list.begin(); it != vertex_list.end(); ++it) {
		Vertex *v = *(*it);
		if(v && v->getName() == name) {
			return *(*it);
		}
	}
	return NULL;
} // }}}

void check_vertex_list() {
	int idx = 0;
	std::string names;
	for(std::vector<Q::iterator>::iterator it = vertex_list.begin(); it != vertex_list.end(); ++it) {
		Vertex *v = *(*it);
		if(NULL == v) {
			printf("Crikey, index %d is null.. others: %s\n", idx, names.c_str());
		}
		idx++;
	}

}

int main() {
	std::ifstream in;
	std::string str;
	int linenum = 0;
	int vertices, edges;
	double running_time;
	std::clock_t start, stop;

	incrCount = compCount = 0;

    start = std::clock();
	// Read graph data from file {{{
	in.open(GRAPHFILE);
	if(!in) {
		printf("Error: cannot open %s\n", GRAPHFILE);
		return 1;
	}

	Q q;

	getline(in,str);
	int edgeNum = 0;
	while(in) {
		if(str[0] == '#')  {
			getline(in,str);
			continue;
		}
		std::stringstream ss(str);
		if(0 == linenum) {
			ss >> vertices;
			ss >> edges;
		} else if(1 == linenum) {
			std::string buf;
			bool first = true;
			int vertId = 0;
			while(ss >> buf) {
				Vertex *v = new Vertex(vertId, buf);
				// Set distance from source to source to 0
				if(first)
					v->setDist(0);
				Q::iterator it = q.push(v);
				vertex_list.push_back(it);
				first = false;
				vertId++;
			}
			printf("Added %d vertices\n", vertId);
		} else {
			std::string buf;
			int vId;
			Vertex *a, *b;
			int w;
			//ss >> buf;
			//a = get_vertex(buf);
			ss >> vId;
			a = *vertex_list.at(vId);
			if(NULL == a) {
				printf("Couldn't find vertex %s\n", buf.c_str());
				return 1;
			}
			//ss >> buf;
			//b = get_vertex(buf);
			ss >> vId;
			b = *vertex_list.at(vId);
			if(NULL == b) {
				printf("Couldn't find vertex %s\n", buf.c_str());
				return 1;
			}
			ss >> w;
			a->addEdge(new Edge(a, b, w));
			if((edgeNum % 100000) == 0) {
				//printf("Added edge %d\n", edgeNum);
			}
			edgeNum++;
		}
		linenum++;
		getline(in,str);
	}
	// END reading dijkstra.dat   }}}
    stop = std::clock();
    running_time = double(stop - start)/double(CLOCKS_PER_SEC);
	printf("Read %d vertices and %d edges in %f from %s.\n", vertex_list.size(), edges, running_time, GRAPHFILE);

    start = std::clock();
	int iter = 0;
	// Run Dijkstra {{{
	while(0 < q.size()) {
		Q::iterator it = q.top();
		Vertex *u = *it;
		q.pop();
		u->setExtracted(true);
		//printf("Popped vertex %s with index %d\n", u->getName().c_str(), u->getId());

		std::vector<Edge*> *edges = u->getEdges();
		for(std::vector<Edge*>::iterator it = edges->begin(); it != edges->end(); ++it) {
			Edge *e = *it;
			Vertex *v = e->getToVertex();
			int alt = u->getDist() + e->getWeight();
			if(alt < v->getDist()) {
				// Relax edge
				if(!v->isExtracted()) {
					v->setDist(alt);
					v->setPrev(u);
					Q::iterator it = vertex_list.at(v->getId());
					q.increase(it, v);
					++incrCount;
					//printf("Relaxed edge from %s to %s to %d\n", u->getName().c_str(), v->getName().c_str(), v->getDist());
				}
			}
		}
		iter++;
	} // }}}
    stop = std::clock();
    running_time = double(stop - start)/double(CLOCKS_PER_SEC);
	printf("Found single-source shortest paths in %f.\n", running_time);
	printf("Used %d comparisons, %d increase ops and %d extract ops.\n", compCount, incrCount, vertex_list.size());

	printf("Size of pqueue: %d\n", q.size());
	printf("Result path:\n");

	return 0;
	int idx = 0;
	for(std::vector<Q::iterator>::iterator it = vertex_list.begin(); it != vertex_list.end(); ++it) {
		Vertex *v = *(*it);
		if(NULL == v) {
			printf("Hmm, index %d is null...\n", idx++);
			continue;
		}
		if(NULL == v->getPrev())
			printf("Source: %s\n", v->getName().c_str());
		else
			printf("  Path: %s -> %s\n", v->getPrev()->getName().c_str(), v->getName().c_str());
		idx++;
	}
}

