
#define PAIR

#ifdef PAIR
#include "priority-queue-pair.c++"
typedef cphstl::priority_queue_pair<int> PQ;
#endif

#ifdef PAIR_LAZY
#include "priority-queue-pair-lazy.c++"
typedef cphstl::priority_queue_pair_lazy<int> PQ;
#endif

#ifdef COSTLESS_MELD
#include "priority-queue-costless-meld.c++"
typedef cphstl::priority_queue_costless_meld<int> PQ;
#endif


using namespace cphstl;
#define _V int
#define _A std::allocator<_V>
#define _E heap_node<_V, _A>

int main() {
        std::less<_V> c = std::less<_V>();
        _A a = _A();

				PQ* pq = new PQ(c, a);

        assert(pq->size() == 0);

        const int N = 1024*1024*16;

        int i;
        _E* node;

        printf("INSERT\n");
        for(i = N; i > 0; i-=4) {
                int v = i;
                //printf("insert: %i\n", v);
                pq->insert(new _E(v,a));
                pq->insert(new _E(v+2,a));
                node = new _E(v,a);
                pq->insert(node);
                pq->insert(new _E(v+3,a));

                pq->increase(node, v+1);
                //pq.show();

                assert(pq->size() == N-i+4);
        }

        printf("EXTRACT\n");
        node = pq->extract();

        _V prev = node->element();
        int count = 1;
        while(pq->size() > 0) {
                node = pq->extract();
                assert(node != NULL);
                //printf("extract: %i\n", node->element());
                assert(node->element() < prev);
                prev = node->element();
                ++count;
        }

				printf("%i %i\n", count, N);
        assert(count == N);

        printf("done.\n");
}

