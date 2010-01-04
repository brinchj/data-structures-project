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

                typedef size_t size_type;

                priority_queue_pair_lazy(C const& c, A const& a)
                        : comparator(c), allocator(a), top_(NULL), size_(0) {
                }

                ~priority_queue_pair_lazy() {
                        // precondition: The data structure contains no elements
                }

                /* Insert element */
                void insert(E* p) {
                        // heap is empty
                        if(size_ == 0) {
                                top_ = p;
                                size_ = 1;
                                return;
                        }
                        if(auxlist == NULL) {
                                auxlist = auxlist_last = p;
                        } else {
                                p->left_  = auxlist_last;
                                p->right_ = NULL;
                                auxlist_last->right_ = p;
                                auxlist_last = p;
                        }

                        ++size_;
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
                        if(p == top_) return;

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
                        top_ = meld_nodes(top_, p);
                }

                E* extract() {
                        //printf("extract: %ld\n", size_);
                        if(size_ == 0) {
                                return NULL;
                        } else if (size_ == 1) {
                                size_ = 0;
                                return top_;
                        }


                        // process waiting nodes
                        while(auxlist != auxlist_last) {
                                // cut first two elements from list
                                E* first = auxlist;
                                auxlist = auxlist->right_->right_;

                                // meld first two nodes
                                E* node = meld_nodes(first, first->right_);
                                // insert new node as last in list
                                if(auxlist == NULL) {
                                        auxlist = node;
                                        break;
                                } else {
                                        node->left_ = auxlist_last;
                                        node->right_ = NULL;
                                        auxlist_last->right_ = node;
                                        auxlist_last = node;
                                }
                        }

                        if(auxlist != NULL) {
                                top_ = meld_nodes(top_, auxlist);
                                auxlist = auxlist_last = NULL;
                        }


                        E* min = top_;
                        E* list = NULL;

                        // iterate through children
                        E* node = top_->child_;
                        while(node != NULL) {
                                if(node->right_ == NULL) {
                                        break;
                                }
                                E* next = node->right_->right_;
                                // merge with first in list
                                node = meld_nodes(node, node->right_);
                                // set merged node as first in list
                                node->right_ = list;
                                list = node;
                                // move to next child
                                node = next;
                        }

                        // prepend last element to list
                        if(node != NULL) {
                                node->right_ = list;
                                list = node;
                        }

                        // merge element in list
                        top_ = list;
                        node = list->right_;
                        while(node != NULL) {
                                E* next = node->right_;
                                top_ = meld_nodes(top_, node);
                                node = next;
                        }

                        --size_;
                        return min;
                }

        private:
                size_type size_;
                C comparator;
                A allocator;
                E* top_;
                E* auxlist;
                E* auxlist_last;
        };
}

#endif
