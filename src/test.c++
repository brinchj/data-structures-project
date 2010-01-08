
#include <stdio.h>

#define PAIR


#define _V int
#define _A std::allocator<_V>
#define _E weak_heap_node<_V, _A>


class comparator {
public:
  comparator() {
      count_ = 0;
    }

  bool operator()(_V const& a, _V const& b) {
    count_ = count_ + 1;
    return a < b;
  }

  unsigned int get_count() {
    return count_;
  }

private:
  unsigned int count_;
};


#ifdef PAIR
#include "priority-queue-pair.c++"
typedef cphstl::priority_queue_pair<int,comparator> PQ;
#endif

#ifdef PAIR_LAZY
#include "priority-queue-pair-lazy.c++"
typedef cphstl::priority_queue_pair_lazy<int,comparator> PQ;
#endif

#ifdef COSTLESS_MELD
#include "priority-queue-costless-meld.c++"
typedef cphstl::priority_queue_costless_meld<int,comparator> PQ;
#endif


using namespace cphstl;

int main() {
  comparator c = comparator();
  _A a = _A();

  PQ* pq = new PQ(c, a);

  assert(pq->size() == 0);

  const int N = 1024*1024*8;
  const int M = 1024;

  int i,j;
  _E* node;

  printf("INSERT\n");
  for(i = N; i > 0; i-=M) {
    int v = i;
    _E *nodes[M];
    for(j = 0; j < M; j++) {
      nodes[j] = new _E(0, a);
      pq->insert(nodes[j]);
    }
    for(j = 0; j < M; j++) {
      pq->increase(nodes[j], M-j);
    }
    for(j = 0; j < M; j++) {
      //printf("%i %i\n", M-j, N-i-j+M+1);
      pq->increase(nodes[j], N-i-j+M+1);
    }
    assert(pq->size() == N-i+M);
  }

  printf("EXTRACT\n");
  node = pq->extract();
  //printf("extract: %i\n", node->element());

  _V prev = node->element();
  int count = 1;
  while(pq->size() > 0) {
    free(node);
    node = pq->extract();
    assert(node != NULL);
    //printf("extract: %i\n", node->element());
    assert(node->element() < prev);
    prev = node->element();
    ++count;
  }
  free(node);

  printf("comparisons: %i\n", pq->comparator.get_count());
  assert(count == N);

  free(pq);
  printf("done.\n");
}

