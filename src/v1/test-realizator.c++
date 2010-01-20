#include <stdlib.h>
#include <stdio.h>

#define DEBUG


#include "stl-meldable-priority-queue.h++"

#include "pairing-heap-node.c++"
#include "pairing-heap-framework.h++"

#include "pairing-heap-policy-strict.c++"
#include "pairing-heap-policy-lazy-insert.c++"
#include "pairing-heap-policy-lazy-increase.c++"
#include "pairing-heap-policy-lazy-insert-increase.c++"

long long comps = 0;

template <typename T>
class counting_comparator {
public:

  bool operator()(T const& a, T const& b) const {
    ++comps;
    return a < b;
  }
};


typedef int _V;
//typedef std::less<_V> _C;
typedef counting_comparator<_V> _C;
typedef std::allocator<_V> _A;
typedef cphstl::pairing_heap_node<_V, _A, _C>  _E;

//typedef cphstl::pairing_heap_policy_strict<_V, _C, _A, _E>  P;
//typedef cphstl::pairing_heap_policy_lazy_insert<_V, _C, _A, _E>  P;
//typedef cphstl::pairing_heap_policy_lazy_increase<_V, _C, _A, _E>  P;
typedef cphstl::pairing_heap_policy_lazy_insert_increase<_V, _C, _A, _E>  P;
typedef cphstl::pairing_heap_framework<_V, P, _C, _A, _E> PQ;

typedef cphstl::meldable_priority_queue<_V, _C, _A, _E, PQ> MPQ;


using namespace cphstl;
int main() {

  const int N = 128;

  MPQ pq = MPQ();

  _A a = _A();
  _C c = _C();

  //P p = P(c, a);

  //PQ* pq = new PQ(c, a);
  _E* nodes[N];

  printf("INSERT\n");
  int i;
  for(i = 0; i < N; i++) {
    nodes[i] = pq.push(i);
    //pq.is_valid();
  }

  printf("INCREASE\n");
  for(i = 0; i < N; i++) {
    _E* node = nodes[i];
    pq.increase(node, node->element()+ (i%2)*10 + 1);
    //pq.is_valid();
  }

  printf("EXTRACT\n");
  _E *node, *prev;
  for(i = 0; i < N; i++) {
    node = pq.top();
    int v = node->element();
    pq.pop();
    //printf("node: %i\n", v);
    if(prev != NULL) {
      //assert(node->element() < prev->element());
    }
    prev = node;
    //pq.is_valid();
  }

  printf("comparisons: %lld\n", comps);
  printf("done.\n");
}
