
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
  typedef pairing_heap_policy_lazy_increase<V,C,A,E> P;

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

  E* begin() const {
    return NULL;
  }

  E* end() const {
    return NULL;
  }



  /* Insert element */
  void insert(E **top, E **min, E* p) {
    // Precondition: Heap is not empty
    *min = *top = (*top)->meld( p );
  }

  /* Increase value of element */
  void increase(E **top, E **min, E* p, V const& v) {
    p->value_ = v;

    // update minimum
    if(*min == NULL ||
       comparator_((*min)->element(), p->element())) {
      *min = p;
    }

    // if p is top we're done
    if(p == *top || p->color_ == 1) {
      return;
    }

    // insert p into list
    p->color_ = 1;
    list_.push_front(p);
    p->node_in_list_ = list_.begin();
    assert(p->node_in_list_->content() == p);
  }


  int is_valid_tree(E *root, bool undo=false) {
    if(root == NULL) {
      return 0;
    }

    E* prev = root;
    E* node = root->child_;

    int count = 0;
    while(node != NULL) {
      assert(node->left_ == prev);
      if(undo) {
        assert(node->color_ >= 256);
        node->color_ &= 255;
      } else {
        assert(node->color_ < 256);
        node->color_ |= 256;
      }
      assert(root->value_ >= node->value_);

      count += 1 + is_valid_tree(node, undo);

      prev = node;
      node = node->right_;
    }
    return count;
  }



  E* extract(E **top, E **min, int size) {
    // Precondition: at least two elements in queue, size > 1

    if((*min)->color_ == 1) {
      assert((*min)->node_in_list_ != NULL);
    }
    assert((*top)->color_ == 0);

    E* extracted_node = NULL;
    if((*min)->color_ == 1) {
      extracted_node = *min;
    }

    if(list_.size() > 0) {
      cleanup(top, min, size);
    }

    if(extracted_node != NULL) {
      (*top)->swap(extracted_node);
    } else {
      extracted_node = *top;
    }
    E* list = NULL;

    // iterate through children
    E* node = (*top)->child_;
    while(node != NULL && node->right_ != NULL) {
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
      assert(node->color_ != 1);
      node->right_ = list;
      list = node;
    }

    // merge element in list
    if(list != NULL) {
      *top = list;
      node = list->right_;
      while(node != NULL) {
        E* next = node->right_;
        *top = (*top)->meld( node );
        node = next;
      }
      (*top)->left_ = (*top)->right_ = NULL;
    } else {
      *top = NULL;
    }

    *min = *top;
    assert((*top)->color_ == 0);

    return extracted_node;
  }


  E* extract(E **top, E **min, E *p) {

    if(p->color_ == 1) {
      // cut p from list
      list_.erase(p->node_in_list_);
      p->color_ = 0;
    }

    // cut p from tree
    p->tree_cut(top);

    // reinsert children
    E* node = p->child_;
    while(node != NULL) {
      E* next = node->right_;
      *top = (*top)->meld( node );
      node = next;
    }

    p->child_ = NULL;
    return p;
  }


  void cleanup(E **top, E **min, int size) {
    // size of list to sort
    const size_t logN = ceil(log(size));

    // comparator
    node_compare<V,C,A,E>* ncmp = new node_compare<V,C,A,E>(comparator_);
    doubly_linked_list<E*> sort_list;

    list_node<E*> *node;
    while(list_.size() > 0) {
      node = list_.begin();
      E* p = node->content();

      list_.erase(node);
      p->color_ = 0;
      p->node_in_list_ = NULL;

      assert(p != NULL);

      // cut out left child
      if(p->child_ != NULL) {
        E* myleft = p->child_;
        p->child_ = myleft->right_;
        if(p->child_) {
          p->child_->left_ = p;
        }
        // insert myleft in p's place
        p->replace_with(myleft);
      } else {
        p->tree_cut(top);
      }
      sort_list.push_front(p);
      if(sort_list.size() == logN) {
        sort_list.sort(*ncmp);
        list_node<E*> *mynode;
        E* tree = sort_list.begin()->content();
        sort_list.erase(sort_list.begin());
        while(sort_list.size() > 0) {
          mynode = sort_list.begin();
          tree = (mynode->content())->meld(tree, true);
          sort_list.erase(mynode);
        }
        *top = (*top)->meld( tree );
      }
    }
    if(sort_list.size() > 0) {
      sort_list.sort(*ncmp);
      list_node<E*> *mynode;
      E* tree = sort_list.begin()->content();
      sort_list.erase(sort_list.begin());
      while( sort_list.size() ) {
        mynode = sort_list.begin();
        tree = (mynode->content())->meld(tree, true);
        sort_list.erase(mynode);
      }
      *top = (*top)->meld( tree );
    }
    *min = *top;
  }


  void meld(E **top, E **min, pairing_heap_framework<V,P,C,A,E> &other) {
  }


  int is_valid() {
    return 0;
  }


private:
  C comparator_;
  A allocator_;

  doubly_linked_list<E*> list_;

};
}
#endif

