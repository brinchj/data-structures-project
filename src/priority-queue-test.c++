/*
  A priority-queue framework can be used to generate a realizator for
  a meldable priority queue. The efficiency of different operations
  depends on the policies relied on.

  Author: Jyrki Katajainen © 2009
*/

#include "priority-queue.h++"

namespace cphstl {
        template <
                typename V,
                typename C = std::less<V>,
                typename A = std::allocator<V>,
                typename E = heap_node<V, A>,
                typename P = perfect_component<E>
                >
        class priority_queue_test : public priority_queue<V,C,A,E,P> {
        public:
				void show_heap(E* node, int lvl) {
						if(node == NULL) return;
						printf("> %i -> %i\n", lvl, node->element());
						E* child = node->child_;
						while(child != NULL) {
								show_heap(child, lvl+1);
								child = child->right_;
						}
				}

				void show() {
						printf(">> HEAP:\n");
						show_heap(this->top_, 0);
						printf("<<\n");
				}
		}
}


