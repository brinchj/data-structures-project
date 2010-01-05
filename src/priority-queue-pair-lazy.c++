#include "priority-queue-pair.c++"

#include <stdio.h>
#include <vector>



//#define DEBUG


#ifndef PRIORITY_QUEUE_PAIR_LAZY
#define PRIORITY_QUEUE_PAIR_LAZY


namespace cphstl {
        template <
				typename V,
                typename C = std::less<V>,
                typename A = std::allocator<V>,
                typename E = heap_node<V, A>,
                typename P = perfect_component<E>
                >
        class priority_queue_pair_lazy : public priority_queue_pair<V,C,A,E,P> {
        public:

								typedef priority_queue<V,C,A,E,P> PQ;
								typedef priority_queue_pair<V,C,A,E,P> PQP;


                priority_queue_pair_lazy(C const& c, A const& a)
                        : comparator(c), allocator(a) {
												PQ::top_ = NULL;
												PQ::size_ = 0;
                }

                ~priority_queue_pair_lazy() {
                        // precondition: The data structure contains no elements
                }

                /* Insert element */
                void insert(E* p) {
                        // heap is empty
                        if(PQ::size_ == 0) {
                                PQ::top_ = p;
                                PQ::size_ = 1;
                                return;
                        }
                        if(PQ::auxlist == NULL) {
                                PQ::auxlist =
                                        PQ::auxlist_last = p;
                        } else {
                                p->left_  = PQ::auxlist_last;
                                p->right_ = NULL;
                                PQ::auxlist_last->right_ = p;
                                PQ::auxlist_last = p;
                        }

                        PQ::size_ += 1;
                        //show();
                        return;
                }

                /* Increase value of element */
                void increase(E* p, V const& v) {
                        //printf("INCR: %i -> %i\n", p->element(), v);

                        ASSERT(p != NULL);
                        ASSERT(comparator(p->element(), v));

                        p->value_ = v;

                        // if p is top we're done
                        if(p == PQ::top_) return;

                        // remove p from child-list
                        if(p->left_->child_ && p->left_->child_==p) {
                                // p is left-most child (left is parent)
                                p->left_->child_ = p->right_;
                                if(p->right_) {
                                        p->right_->left_ = p->left_;
                                }
                        } else if (p->right_ == NULL) {
                                // p is right-most child
                                p->left_->right_ = NULL;
                        } else {
                                // p is somewhere inside child list
                                p->left_->right_ = p->right_;
                                p->right_->left_ = p->left_;
                        }

                        // reinsert p
                        p->left_ = p->right_ = NULL;

												// insert last in auxlist
												insert(p);
												PQ::size_ -= 1;
                }

                E* extract() {
                        //printf("extract: %ld\n", PQ::size_);
                        if(PQ::size_ == 0) {
                                return NULL;
                        } else if (PQ::size_ == 1) {
                                PQ::size_ = 0;
                                return PQ::top_;
                        }


                        // process waiting nodes
                        while(PQ::auxlist != PQ::auxlist_last) {
                                // cut first two elements from list
                                E* first = PQ::auxlist;
                                PQ::auxlist =
                                        PQ::auxlist->right_->right_;

                                // meld first two nodes
                                E* node = meld_nodes(first, first->right_);
                                // insert new node as last in list
                                if(PQ::auxlist == NULL) {
                                        PQ::auxlist = node;
                                        break;
                                }
                                node->left_ = PQ::auxlist_last;
                                node->right_ = NULL;
                                PQ::auxlist_last->right_ = node;
																PQ::auxlist_last = node;
                        }

                        if(PQ::auxlist != NULL) {
                                PQ::top_ = meld_nodes(PQ::top_, PQ::auxlist);
                                PQ::auxlist = PQ::auxlist_last = NULL;
                        }

												return PQP::extract();
                }

				protected:
                C comparator;
                A allocator;
        };
}

#endif
