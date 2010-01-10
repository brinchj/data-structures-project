#include <stdio.h>
#include <vector>

#include "pairing-heap-node.c++"


#ifndef PAIRING_HEAP_POLICY_LAZY_INSERT
#define PAIRING_HEAP_POLICY_LAZY_INSERT

namespace cphstl {
  template <
    typename V,
    typename C = std::less<V>,
    typename A = std::allocator<V>,
    typename E = pairing_heap_node<V, A>
    >
  class pairing_heap_policy_lazy_insert {
  public:

    // types
    typedef std::size_t size_type;
    typedef pairing_heap_policy_lazy_insert<V,C,A,E> P;

    pairing_heap_policy_lazy_insert(C const& c, A const& a)
      : comparator_(c), allocator_(a) {
      list_ = list_end_ = NULL;
    }

    pairing_heap_policy_lazy_insert() {
      comparator_ = C();
      allocator_  = A();
    }

    ~pairing_heap_policy_lazy_insert() {
      // precondition: The data structure contains no elements
    }

    E* begin() const { return NULL; }
    E* end() const { return NULL; }


    /* Insert element */
    void insert(E **top, E **min, E* p) {
      // Precondition: Heap is not empty
      if(list_ == NULL) {
        list_ = list_end_ = p;
      } else {
        p->left_  = list_end_;
        p->right_ = NULL;
        list_end_->right_ = p;
        list_end_ = p;
      }
      // set color
      p->color_ = 1;
      // update minimum
      if(*min == NULL ||
         comparator_((*min)->element(), p->element())) {
        *min = p;
      }
    }

    /* Increase value of element */
    void increase(E **top, E **min, E* p, V const& v) {
      //printf("INCR: %i -> %i\n", p->element().value, v.value);

      p->value_ = v;

      // if p is top we're done
      if(p == *top || p->color_ == 1) {
        if(*min == NULL ||
           comparator_((*min)->element(), p->element())) {
          *min = p;
        }
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
      insert(top, min, p);
    }

    E* extract(E **top, E **min) {
      // Precondition: at least two elements in queue, size > 1

      // process waiting nodes
      while(list_ != list_end_) {
        // cut first two elements from list
        E* first = list_;
        list_ = list_->right_->right_;

        // meld first two nodes
        first->color_ = first->right_->color_ = 0;
        E* node = first->meld( first->right_ );
        // insert new node as last in list
        if(list_ == NULL) {
          list_ = node;
          break;
        }
        node->left_ = list_end_;
        node->right_ = NULL;
        list_end_->right_ = node;
        list_end_ = node;
      }

      if(list_ != NULL) {
        list_->color_ = 0;
        *top = (*top)->meld( list_ );
        list_ = list_end_ = NULL;
      }

      // pick top for extraction
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
      if(list != NULL) {
        node = list->right_;
        while(node != NULL) {
          E* next = node->right_;
          *top = (*top)->meld( node );
          node = next;
        }
        (*top)->left_ = (*top)->right_ = NULL;
      }

      *min = *top;
      return extracted_node;
    }


    void extract(E **top, E **min, E *p) {
      // Precondition: p is not min

      if(p->color_ == 1) {
        // p is in list
        if(list_ == list_end_) {
          list_ = list_end_ = NULL;
        } else if (p == list_) {
          list_ = list_->right_;
        } else if (p == list_end_) {
          list_end_->left_->right_ = NULL;
        } else {
          p->left_->right_ = p->right_;
          p->right_->left_ = p->left_;
        }
        return;
      }

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
      assert(list != NULL);
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

    E *list_, *list_end_;

  };
}
#endif

