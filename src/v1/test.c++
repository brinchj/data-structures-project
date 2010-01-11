#include <stdlib.h>
#include <stdio.h>

#include "pairing-heap-node.c++"
#include "pairing-heap-framework.h++"
#include "pairing-heap-policy-strict.c++"
#include "pairing-heap-policy-lazy-insert.c++"
#include "pairing-heap-policy-lazy-increase.c++"



typedef int _V;
typedef std::less<_V> _C;
typedef std::allocator<_V> _A;
typedef cphstl::heap_node<_V, _A>  _E;

//typedef cphstl::pairing_heap_policy_strict<_V>  P;
typedef cphstl::pairing_heap_policy_lazy_insert<_V>  P;
//typedef cphstl::pairing_heap_policy_lazy_increase<_V>  P;
typedef cphstl::pairing_heap_framework<_V, P> PQ;



using namespace cphstl;
int main() {

  const int N = 8;


  _A a = _A();
  _C c = _C();

  P p = P(c, a);

  PQ* pq = new PQ(c, a, p);
  _E* nodes[N];

  printf("INSERT\n");
  int i;
  for(i = 0; i < N; i++) {
    nodes[i] = new _E(i, a, c);
    pq->insert(nodes[i]);
    //pq->is_valid();
  }

  printf("INCREASE\n");
  for(i = 0; i < N; i++) {
    _E* node = nodes[i];
    pq->increase(node, node->element()+ (i%2)*10 + 1);
    //pq->is_valid();
  }

  printf("EXTRACT\n");
  _E *node, *prev;
  for(i = 0; i < N; i++) {
    node = pq->extract();
    printf("node: %i\n", node->element());
    if(prev != NULL) {
      //assert(node->element() < prev->element());
    }
    prev = node;
    //pq->is_valid();
  }

  printf("done.\n");
}
