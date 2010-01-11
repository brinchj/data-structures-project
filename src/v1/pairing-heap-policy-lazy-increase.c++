
#ifndef PAIRING_HEAP_POLICY_LAZY_INCREASE
#define PAIRING_HEAP_POLICY_LAZY_INCREASE

#include <stdio.h>
#include <vector>
#include <math.h>

#include "pairing-heap-node.c++"
#include "doubly-linked-list.h++"

namespace cphstl {

  template <
    typename V,
    typename C = std::less<V>,
    typename A = std::allocator<V>,
    typename E = pairing_heap_node<V, A, C>
    >
  class node_compare {
  public:
    node_compare(C const& c) : comparator_(c) {}
    bool operator()(E* a, E* b) {
      return comparator_(a->element(), b->element());
    }
  protected:
    C comparator_;
  };

template <
  typename V,
  typename C = std::less<V>,
  typename A = std::allocator<V>,
  typename E = pairing_heap_node<V, A, C>
  >
class pairing_heap_policy_lazy_increase {
public:

  // types
  typedef std::size_t size_type;

  pairing_heap_policy_lazy_increase(C const& c, A const& a)
    : comparator_(c), allocator_(a), list_(NULL) {
  }

  pairing_heap_policy_lazy_increase() {
    comparator_ = C();
    allocator_  = A();
  }


  ~pairing_heap_policy_lazy_increase() {
    // precondition: The data structure contains no elements
  }

  /* Insert element */
  void insert(E **top, E **min, E* p) {
    // Precondition: Heap is not empty
    *min = *top = (*top)->meld( p );
  }

  /* Increase value of element */
  void increase(E **top, E **min, E* p, V const& v) {
    //printf("INCR: %i -> %i\n", p->element().value, v.value);
    p->value_ = v;

    // update minimum
    if(*min == NULL ||
       comparator_((*min)->element(), p->element())) {
      *min = p;
    }

    // if p is top we're done
    if(p == *top || p->color_ == 1) return;

    // insert p into list
    p->color_ = 1;
    list_.push_front(p);
  }

  E* extract(E **top, E **min) {
    // Precondition: at least two elements in queue, size > 1

    if(list_.size() > 0) {
      cleanup(top, min, 0);
    }

    E* extracted_node = *top;
    E* list = NULL;

    // iterate through children
    E* node = (*top)->child_;
    while(node != NULL) {
      if(node->right_ == NULL) {
        break;
      }
      assert(node != NULL && node->right_ != NULL);
      E* next = node->right_->right_;
      // merge with first in list
      node = node->meld( node->right_ );
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
    *top = list;
    node = list->right_;
    while(node != NULL) {
      E* next = node->right_;
      *top = (*top)->meld( node );
      node = next;
    }

    (*top)->left_ = (*top)->right_ = NULL;
    *min = *top;
    return extracted_node;
  }


  void cleanup(E **top, E** min, int size) {
    // size of list to sort
    const int N = ceil(log(size));

    // comparator
    node_compare<V,C,A,E>* ncmp =
      new node_compare<V,C,A,E>(comparator_);
    doubly_linked_list<E*> sort_list;

    list_node<E*> *node;
    list_node<E*> *sentinel = list_.end();
    while( (node = list_.begin()) != sentinel) {
      list_.erase(node);
      node->content()->color_ = 0;

      E* p = node->content();
      assert(p != NULL);

      // cut out left child
      if(p->child_ != NULL) {
        //printf("has left child\n");
        E* myleft = p->child_;
        p->child_ = myleft->right_;
        if(p->child_) {
          p->child_->left_ = p;
        }
        // insert myleft in p's place
        if(p->left_->child_ && p->left_->child_==p) {
          // p is left-most child (left is parent)
          //printf("left-most\n");
          p->left_->child_ = myleft;
          myleft->left_ = p->left_;
          if(p->right_ != NULL) {
            p->right_->left_ = myleft;
          }
          myleft->right_ = p->right_;
        } else if (p->right_ == NULL) {
          //printf("right-most\n");
          // p is right-most child
          p->left_->right_ = myleft;
          myleft->left_ = p->left_;
          myleft->right_ = NULL;
        } else {
          // p is somewhere inside child list
          //printf("inside\n");
          p->left_->right_ = myleft;
          p->right_->left_ = myleft;
          myleft->left_ = p->left_;
          myleft->right_ = p->right_;
        }
      } else {
        //printf("no child\n");
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
        sort_list.push_front(p);
        if(sort_list.size() == N) {
          sort_list.sort(*ncmp);
          list_node<E*> *mynode;
          E* tree = sort_list.begin()->content();
          sort_list.erase(sort_list.begin());
          while( (mynode = sort_list.begin()) != sort_list.end() ) {
            sort_list.erase(mynode);
            tree = tree->meld( mynode->content() );
          }
          *top = (*top)->meld( tree );
        }
      }
      if(sort_list.size() > 0) {
        sort_list.sort(*ncmp);
        list_node<E*> *mynode;
        E* tree = sort_list.begin()->content();
        sort_list.erase(sort_list.begin());
        while( (mynode = sort_list.begin()) != sort_list.end() ) {
          sort_list.erase(mynode);
          tree = meld_nodes(tree, mynode->content());
        }
        *top = (*top)->meld( tree );
      }
      *min = *top;
    }
  }


private:
  C comparator_;
  A allocator_;

  doubly_linked_list<E*> list_;

};
}
#endif

