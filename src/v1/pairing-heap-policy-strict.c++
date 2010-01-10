#include <stdio.h>
#include <vector>

#include "pairing-heap-node.c++"


#ifndef PAIRING_HEAP_POLICY_STRICT
#define PAIRING_HEAP_POLICY_STRICT

namespace cphstl {
  template <
    typename V,
    typename C = std::less<V>,
    typename A = std::allocator<V>,
    typename E = pairing_heap_node<V, A>
    >
  class pairing_heap_policy_strict {
  public:

    // types
    typedef std::size_t size_type;
    typedef pairing_heap_policy_strict<V,C,A,E> P;

    pairing_heap_policy_strict(C const& c, A const& a)
      : comparator_(c), allocator_(a) {
    }

    pairing_heap_policy_strict() {
      comparator_ = C();
      allocator_  = A();
    }

    ~pairing_heap_policy_strict() {
      // precondition: The data structure contains no elements
    }

    E* begin() const { return NULL; }
    E* end() const { return NULL; }

    /* Insert element */
    void insert(E **top, E **min, E* p) {
      // Precondition: Heap is not empty
      *min = *top = (*top)->meld( p );
    }

    /* Increase value of element */
    void increase(E **top, E **min, E* p, V const& v) {
      //printf("INCR: %i -> %i\n", p->element().value, v.value);
      p->value_ = v;

      // if p is top we're done
      if(p == *top) {
        *min = *top;
        return;
      }

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
      *min = *top = (*top)->meld( p );
    }

    E* extract(E **top, E **min) {
      // Precondition: at least two elements in queue, size > 1

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

    void extract(E **top, E **min, E *p) {
      // Precondition: p is not min

      if(p->child_ == NULL) {
        if(p->left_->child_ == p) {
          p->left_->child_ = p->right_;
        } else {
          p->left_->right_ = p->right_;
        }
        if(p->right_ != NULL) {
          p->right_->left_ = p->left_;
        }
        return;
      }

      // iterate through children
      E* list = NULL;
      E* node = p->child_;
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
      E* root = list;
      node = list->right_;
      while(node != NULL) {
        E* next = node->right_;
        root = root->meld( node );
        node = next;
      }

      // extract element from child list
      root->right_ = p->right_;
      root->left_  = p->left_;
      if(p->left_->child_ == p) {
        p->left_->child_ = root;
      } else {
        p->left_->right_ = root;
      }
      if(p->right_ != NULL) {
        p->right_->left_ = root;
      }
    }


    void meld(E **top, E **min,
              cphstl::pairing_heap_framework<V,P,C,A,E>& other) {
    }


  private:
    C comparator_;
    A allocator_;

  };
}
#endif

