#include <iostream> // std::cout
#include <fstream>
#include <utility> // std::pair
#include <sstream>
#include <vector>
#include <list>
#include <stdio.h>

#include "stl-meldable-priority-queue.h++"

#include "pairing-heap-framework.h++"
#include "pairing-heap-policy-strict.c++"


#define INF 1024*1024*2

using namespace cphstl;
#define _V Vertex*
#define _A std::allocator<_V>
#define _E heap_node<_V, _A>

//class vertex_comparator {
//public:
//  
//  bool operator()(T const& a, T const& b) const {
//    ++comps;
//    return a < b;
//  }
//}

class Vertex;

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
			dist = INF;
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
			// XXX: It's a max heap so this was the fastest way to "fix" it ;-)
			return dist < __v.dist;
		}
	private:
		int dist;
		int id;
		bool extracted;
		Vertex *previous;
		std::string name;
		std::vector<Edge*> adjacent_edges;
}; // }}}

typedef Vertex* V;
typedef std::greater<V> C;
typedef std::allocator<V> A;
typedef cphstl::pairing_heap_node<V, A, C> E;
typedef cphstl::pairing_heap_policy_strict<V, C, A, E> POL;
typedef cphstl::pairing_heap_framework<V, POL, C, A, E> PQ;
typedef cphstl::meldable_priority_queue<V, C, A, E, PQ> Q;
#if 0
typedef cphstl::meldable_priority_queue<V, C> Q;
#endif

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

	// Read dijkstra.dat {{{
	in.open("dijkstra.dat");
	if(!in) {
		printf("Error: cannot open dijkstra.dat\n");
		return 1;
	}

	Q q;

	getline(in,str);
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
		} else {
			std::string buf;
			Vertex *a, *b;
			int w;
			ss >> buf;
			a = get_vertex(buf);
			ss >> buf;
			b = get_vertex(buf);
			ss >> w;
			a->addEdge(new Edge(a, b, w));
		}
		linenum++;
		getline(in,str);
	}
	printf("Read %d vertices and %d edges.\n", vertex_list.size(), edges);
	// END reading dijkstra.dat   }}}

	while(0 < q.size()) {
		Q::iterator it = q.top();
		Vertex *u = *it;
		q.pop();
		u->setExtracted(true);
		printf("Popped vertex %s with index %d\n", u->getName().c_str(), u->getId());

		std::vector<Edge*> *edges = u->getEdges();
		for(std::vector<Edge*>::iterator it = edges->begin(); it != edges->end(); ++it) {
			Edge *e = *it;
			Vertex *v = e->getToVertex();
			int alt = u->getDist() + e->getWeight();
			if(alt < v->getDist()) {
				// Relax edge
				v->setDist(alt);
				v->setPrev(u);
				if(!v->isExtracted()) {
					Q::iterator it = vertex_list.at(v->getId());
					q.increase(it, v);
				}
			}
		}
	}

	printf("Size of pqueue: %d\n", q.size());
	printf("Result path:\n");

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

