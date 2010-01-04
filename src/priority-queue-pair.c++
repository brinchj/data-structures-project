#include "priority-queue.h++"
#include "priority-queue-node.h++"

#include <stdio.h>
#include <vector>

#define _PQTN typename
#define _PQ_TEMPLATE template <_PQTN V, _PQTN C, _PQTN A, _PQTN E, _PQTN P>
#define _PQ_TYPE     priority_queue<V, C, A, E, P>
#define _PQ_CONS     _PQ_TEMPLATE _PQ_TYPE

#define _PQ_RET(T) _PQ_TEMPLATE typename _PQ_TYPE::T priority_queue<V, C, A, E, P>
#define _PQ_RET_TEMPL(T) _PQ_TEMPLATE T priority_queue<V, C, A, E, P>
#define _PQ_RET_VOID _PQ_RET_TEMPL(void)

namespace cphstl {
		_PQ_CONS::priority_queue(
				C const& c, A const& a)
				: comparator(c), allocator(a), top_(NULL), size_(0) {
		}

		_PQ_CONS::~priority_queue() {
				// precondition: The data structure contains no elements
		}


		// Accessors
		_PQ_RET_TEMPL(E*)::begin() const {
				if(size_ == 0) {
						return NULL;
				}
				return top_;
		}

		_PQ_RET_TEMPL(E*)::end() const {
				return NULL;
		}

		_PQ_RET_TEMPL(A)::get_allocator() const {
				return allocator;
		}

		_PQ_RET_TEMPL(C)::get_comparator() const {
				return comparator;
		}

		_PQ_RET(size_type)::size() const {
				return size_;
		}

		_PQ_RET(size_type)::max_size() const {
				typename std::vector<int, A>::allocator_type a;
				size_type available_memory = a.max_size() * sizeof(int);
				return available_memory / E::footprint();
		}

		_PQ_RET_TEMPL(E*)::top()const {
				return top_;
		}


		// Modifiers

		_PQ_RET_VOID::insert(E* p) {
				//printf("insert: %i\n", p->element());
				// heap is empty
				if(size_ == 0) {
						top_ = p;
						size_ = 1;
						return;
				}
				top_ = meld_nodes(top_, p);
				++size_;
				//show();
				return;
		}

		_PQ_RET_VOID::increase(E* p, V const& v) {
				//printf("INCR: %i -> %i\n", p->element(), v);
				p->value_ = v;

				if(p != top_) {
						// remove p from child-list
						if(p->left_->child_ && p->left_->child_==p) {
								// p is left-most child
								p->left_->child_ = p->right_;
								if(p->right_)
										p->right_->left_ = p->left_;
						} else if (p->right_ == NULL) {
								// p is right-most child
								p->left_->right_ = NULL;
						} else {
								// p is inside child list
								p->left_->right_ = p->right_;
								p->right_->left_ = p->left_;
						}

						// reinsert p
						p->left_ = p->right_ = NULL;
						top_ = meld_nodes(top_, p);
				}

		}

		_PQ_RET_TEMPL(E*)::extract() {
				//printf("extract: %ld\n", size_);
				if(size_ == 0) {
						return NULL;
				} else if (size_ == 1) {
						size_ = 0;
						return top_;
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

		_PQ_RET_VOID::meld(priority_queue& other) {
				size_ += other.size_;
				top_ = meld_nodes(top_, other.top_);
		}

		_PQ_RET_VOID::swap(priority_queue& other) {
				E*        newtop  = other.top_;
				size_type newsize = other.size_;
				other.top_  = newtop;
				other.size_ = newsize;
				top_  = newtop;
				size_ = newsize;
		}
}

using namespace cphstl;


#define _V int
#define _A std::allocator<_V>
#define _E heap_node<_V, _A>

int main() {
		cphstl::priority_queue<int> pq;
		assert(pq.size() == 0);

		const int N = 1024*1024*16;

		_A a = _A();

		int i;
		_E* node;

		printf("INSERT\n");
		for(i = N; i > 0; i-=4) {
				int v = i;
				//printf("insert: %i\n", v);
				pq.insert(new _E(v,a));
				pq.insert(new _E(v+2,a));
				node = new _E(v,a);
				pq.insert(node);
				pq.insert(new _E(v+3,a));

				pq.increase(node, v+1);
				//pq.show();

				//assert(pq.size() == N-i+1);
		}

		printf("EXTRACT\n");
		node = pq.extract();

		_V prev = node->element();
		int count = 1;
		while(pq.size() > 0) {
				node = pq.extract();
				assert(node != NULL);
				//printf("extract: %i\n", node->element());
				assert(node->element() < prev);
				prev = node->element();
				++count;
		}
		assert(count == N);

		printf("done.\n");
}
