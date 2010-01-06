#include "priority-queue.h++"
#include "priority-queue-node.h++"

#include <stdio.h>
#include <vector>

#include "doubly-linked-list.h++"


#define DEBUG

#ifndef PRIORITY_QUEUE_PAIR
#define PRIORITY_QUEUE_PAIR

namespace cphstl {
  template <
    typename V,
    typename C = std::less<V>,
    typename A = std::allocator<V>,
    typename E = heap_node<V, A>,
    typename P = perfect_component<E>
    >
  class priority_queue_pair : public priority_queue<V,C,A,E,P> {
  public:

    typedef priority_queue<V,C,A,E,P> PQ;


    // types
    typedef std::size_t size_type;

    priority_queue_pair(C const& c, A const& a)
      : comparator(c), allocator(a) {
      PQ::top_ = NULL;
      PQ::size_ = 0;
    }

    ~priority_queue_pair() {
      // precondition: The data structure contains no elements
    }

    // Accessors
    E* find_min() {
      return PQ::top_;
    }

    // Modifiers

    /* Insert element */
    void insert(E* p) {
      //printf("insert: %i\n", p->element());
      assert(p != NULL);
      // heap is empty
      if(PQ::size_ == 0) {
        PQ::top_ = p;
        PQ::size_ = 1;
        return;
      }
      PQ::top_ = meld_nodes(PQ::top_, p);
      PQ::size_ += 1;
      //show();
      return;
    }

    /* Increase value of element */
    void increase(E* p, V const& v) {
      //printf("INCR: %i -> %i\n", p->element(), v);

      assert(p != NULL);
      assert(comparator(p->element(), v));

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
      PQ::top_ = meld_nodes(PQ::top_, p);
    }

    E* extract() {
      //printf("extract: %ld\n", PQ::size_);
      if(PQ::size_ == 0) {
        return NULL;
      } else if (PQ::size_ == 1) {
        PQ::size_ = 0;
        return PQ::top_;
      }

      E* min = PQ::top_;
      E* list = NULL;

      // iterate through children
      E* node = PQ::top_->child_;
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
      PQ::top_ = list;
      node = list->right_;
      while(node != NULL) {
        E* next = node->right_;
        PQ::top_ = meld_nodes(PQ::top_, node);
        node = next;
      }

      PQ::size_ -= 1;
      return min;
    }

    void meld(priority_queue_pair& other) {
      PQ::size_ += other.PQ::size_;
      PQ::top_ = meld_nodes(PQ::top_, other.PQ::top_);
    }

    void swap(priority_queue_pair& other) {
      E*        newtop  = other.top_;
      size_type newsize = other.size_;
      other.top_  = newtop;
      other.size_ = newsize;
      PQ::top_  = newtop;
      PQ::size_ = newsize;
    }

    //    protected:
    priority_queue_pair() {}

    void show() {
      printf(">> HEAP:\n");
      show_heap(PQ::top_, 0);
      printf("<<\n");
    }

    void show_heap(E* node, int lvl) {
      if(node == NULL) return;
      printf("> %i -> %i\n", lvl, node->element());
      E* child = node->child_;
      while(child != NULL) {
        show_heap(child, lvl+1);
        child = child->right_;
      }
    }

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
      b->left_ = a;
      a->child_ = b;
      a->left_ = a->right_ = NULL;
      return a;
    }

  protected:
    C comparator;
    A allocator;
  };
}
#endif

