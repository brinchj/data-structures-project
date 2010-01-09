#include <stdlib.h>
#include <stdio.h>

#include "pairing-heap-node.c++"
#include "pairing-heap-framework.h++"
#include "pairing-heap-policy-strict.c++"
#include "pairing-heap-policy-lazy-insert.c++"



typedef int _V;
typedef std::less<_V> _C;
typedef std::allocator<_V> _A;
typedef cphstl::heap_node<_V, _A>  _E;

//typedef cphstl::pairing_heap_policy_strict<_V>  P;
typedef cphstl::pairing_heap_policy_lazy_insert<_V>  P;
typedef cphstl::pairing_heap_framework<_V, P> PQ;



using namespace cphstl;
int main() {

  const int N = 1024*256;


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
    pq->increase(node, node->element()+10);
    //pq->is_valid();
  }

  printf("EXTRACT\n");
  _E* node;
  for(i = 0; i < N; i++) {
    node = pq->extract();
    //pq->is_valid();
  }

  printf("done.\n");
}
