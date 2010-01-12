#include <stdlib.h>
#include <stdio.h>


#define DEBUG


#include "pairing-heap-node.c++"
#include "pairing-heap-framework.h++"
#include "pairing-heap-policy-strict.c++"
#include "pairing-heap-policy-lazy-insert.c++"
#include "pairing-heap-policy-lazy-increase.c++"
#include "stl-meldable-priority-queue.h++"


typedef int _V;
typedef std::less<_V> _C;
typedef std::allocator<_V> _A;
typedef cphstl::pairing_heap_node<_V, _A>  _E;

//typedef cphstl::pairing_heap_policy_strict<_V>  P;
//typedef cphstl::pairing_heap_policy_lazy_insert<_V>  P;
typedef cphstl::pairing_heap_policy_lazy_increase<_V>  P;
typedef cphstl::pairing_heap_framework<_V, P> PQ;
typedef cphstl::meldable_priority_queue<_V, _C, _A, _E, PQ> Q;


using namespace cphstl;
int main() {

  const int N = 8;


  _A a = _A();
  _C c = _C();

  Q pq = Q();
  _E* nodes[N];

  printf("INSERT\n");
  int i;
  for(i = 0; i < N; i++) {
    nodes[i] = pq.push(i);
  }

  printf("INCREASE\n");
  for(i = 0; i < N; i++) {
    _E* node = nodes[i];
    pq.increase(node, node->element()+ (i%2)*10 + 1);
  }

  printf("EXTRACT\n");
  _E *node;
  _V prev;
  for(i = 0; i < N; i++) {
    node = pq.top();
    printf("node: %i\n", node->element());
    prev = node->element();;
    pq.pop();
  }

  printf("done.\n");
}
