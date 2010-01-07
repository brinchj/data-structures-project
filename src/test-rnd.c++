
#include <stdio.h>
#include <stdlib.h>
#define PAIR


class heap_pair {
public:
  explicit heap_pair(int idx, int v) {
    index = idx;
    value = v;
  }

  inline bool
  operator< (heap_pair const& b) const {
    return value < b.value;
  }

  inline bool
  operator<= (heap_pair const& b) const {
    return value <= b.value;
  }

  unsigned int index;
  unsigned int value;
};



#define _V heap_pair
#define _A std::allocator<_V>
#define _E heap_node<_V, _A>


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
typedef cphstl::priority_queue_pair<heap_pair,comparator> PQ;
#endif

#ifdef PAIR_LAZY
#include "priority-queue-pair-lazy.c++"
typedef cphstl::priority_queue_pair_lazy<heap_pair,comparator> PQ;
#endif

#ifdef COSTLESS_MELD
#include "priority-queue-costless-meld.c++"
typedef cphstl::priority_queue_costless_meld<heap_pair,comparator> PQ;
#endif


using namespace cphstl;

int main() {
  srand(101010);

  const int N = 1024*1024*2;
  const int M = 1024;

  comparator c = comparator();
  _A a = _A();

  PQ* pq = new PQ(c, a);

  assert(pq->size() == 0);

  int i,j;
  _E* nodes[N];

  printf("INSERT\n");
  for(i = 0; i < N; i++) {
    nodes[i] = new _E(_V(i, rand() % N), a);
    assert(nodes[i]->element().index == i);
    pq->insert(nodes[i]);
    assert(pq->size() == i+1);
  }

  printf("INCREASE\n");
  int count = 0;
  for(j = 0; j < M; j++) {
    for(i = 0; i < 2*N/M; i++) {
      int idx;
      while( (nodes[idx = rand() % N]) == NULL ) ;
      printf("%i\n", idx);
      assert(nodes[idx]->element().index == idx);
      heap_pair pair = nodes[idx]->element();
      int v = pair.value;
      int r = rand() % N;
      if(v + r > v) {
        heap_pair* new_pair = new _V(idx, v+r);
        pq->increase(nodes[idx], *new_pair);
      }
    }
    heap_pair pair = pq->extract()->element();
    //printf("EXTRACT ONE: %i\n", pair.value);
    free(nodes[pair.index]);
    nodes[pair.index] = NULL;
    count += 1;
  }

  printf("EXTRACT REST\n");
  _E* node = pq->extract();
  count += 1;

  _V prev = node->element();
  while(pq->size() > 0) {
    free(node);
    node = pq->extract();
    assert(node != NULL);
    assert(node->element() <= prev);
    prev = node->element();
    count += 1;
  }
  free(node);

  printf("comparisons: %i\n", pq->comparator.get_count());
  assert(count == N);

  free(pq);
  printf("done.\n");
}

