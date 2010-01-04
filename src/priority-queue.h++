/*
  A priority-queue framework can be used to generate a realizator for
  a meldable priority queue. The efficiency of different operations
  depends on the policies relied on.

  Author: Jyrki Katajainen © 2009
*/

#ifndef __CPHSTL_PRIORITY_QUEUE_FRAMEWORK__
#define __CPHSTL_PRIORITY_QUEUE_FRAMEWORK__

#include <cstddef> // std::size_t and std::ptrdiff_t
#include <functional> // std::less
#include <memory> // std::allocator
#include "priority-queue-node.h++"
#include "perfect-component.h++"

namespace cphstl {
        template <
                typename V,
                typename C = std::less<V>,
                typename A = std::allocator<V>,
                typename E = heap_node<V, A>,
                typename P = perfect_component<E>
                >
        class priority_queue {
        public:

                // types

                typedef V value_type;
                typedef C comparator_type;
                typedef A allocator_type;
                typedef P component_type;
                typedef std::size_t size_type;
                typedef V& reference;
                typedef V const& const_reference;

                // structors

                explicit priority_queue(C const& = C(), A const& = A());
                ~priority_queue();

                // iterators

                E* begin() const;
                E* end() const;

                // accessors

                A get_allocator() const;
                C get_comparator() const;
                size_type size() const;
                size_type max_size() const;
                E* top() const;

                // modifiers

                void insert(E*);
                E* extract();
                void extract(E*);
                void increase(E*, V const&);
                void meld(priority_queue&);
                void swap(priority_queue&);


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
						show_heap(top_, 0);
						printf("<<\n");
				}

        protected:

                C comparator;
                A allocator;
                E* top_;
                size_type size_;

				E* meld_nodes(E* a, E* b) {
						// make sure a is smallest
						if(comparator(a->element(), b->element())){
								E* tmp = a;
								a = b;
								b = tmp;
						}
						// set a as root and b as child
						b->right_ = a->child_;
						if(a->child_) {
								a->child_->left_ = b;
						}
						/*printf("A: %i (%p)\n", a->element(), a);
						  printf("B: %i (%p)\n", b->element(), b);*/
						b->left_ = a;
						a->child_ = b;
						a->left_ = a->right_ = NULL;
						/*printf("a->child: %p\n", a->child_);
						printf("a->right: %p\n", a->right_);
						printf("B->child: %p\n", b->child_);
						printf("B->right: %p\n", b->right_);*/
						return a;
				}

        private:

                priority_queue(priority_queue const&);
                priority_queue& operator=(priority_queue const&);
        };

}

#endif
