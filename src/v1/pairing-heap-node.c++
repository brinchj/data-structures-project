/*
  A heap node used as a base for various specialized heap nodes

  Authors: Asger Bruun, Jyrki Katajainen © 2009
*/

#ifndef __CPHSTL_PAIRING_HEAP_NODE__
#define __CPHSTL_PAIRING_HEAP_NODE__

#include <cstddef> // std::size_t
#include <iostream>
#include <list>

namespace cphstl {

  template <
    typename V,
    typename A = std::allocator<V>,
    typename C = std::less<V>
    >
  class pairing_heap_node {
  public:

    typedef V value_type;
    typedef A allocator_type;
    typedef C comparator_type;
    typedef std::size_t size_type;
    typedef pairing_heap_node<V, A, C> self_t;

    self_t* left_;
    self_t* right_;
    self_t* child_;
    int color_;
    V value_;
    C comparator_;
    A allocator_;

  private:

    pairing_heap_node(pairing_heap_node const&);
    pairing_heap_node& operator=(pairing_heap_node const&);

  public:

    pairing_heap_node()
      : left_(NULL), right_(NULL), child_(NULL), color_(0)
    {
          allocator_  = A();
          comparator_ = C();
    }

    pairing_heap_node(V const& value,
                      std::allocator<pairing_heap_node<A,V,C> > const& a)
      : left_(NULL), right_(NULL), child_(NULL), color_(0)
    {
          allocator_  = A();
          comparator_ = C();
          value_ = value;
    }

    pairing_heap_node(V const& v, A const& a, C const& c)
      : left_(NULL), right_(NULL), child_(NULL), color_(0),
        value_(v), allocator_(a), comparator_(c) {
    }

    static size_type footprint() {
      return sizeof(self_t);
    }


    self_t* successor() const {
      return NULL;
    }

    self_t* owner() const {
      return NULL;
    }

    self_t* root() const {
      return NULL;
    }


    self_t* meld(self_t* b) {
      // make sure we are smallest
      self_t* a = this;
      if(comparator_(value_, b->element())){
        self_t* tmp = this;
        a = b;
        b = tmp;
      }
      // set a as root and b as child
      b->right_ = a->child_;
      if(a->child_ != NULL) {
        a->child_->left_ = b;
      }
      b->left_ = a;
      a->child_ = b;
      a->left_ = a->right_ = NULL;

      return a;
    }


    void list_cut(self_t **list) {
      // cut element from double-linked list
      if(left_ == NULL) {
        // this is first element
        *list = right_;
        if(right_ != NULL)
          right_->left_ = NULL;
      } else if (right_ == NULL) {
        // this is last element
        left_->right_ = NULL;
      } else {
        // this is in between two elements
        left_->right_ = right_;
        right_->left_ = left_;
      }
      left_ = right_ = NULL;
    }

    void tree_cut(self_t **root) {
      if(this == *root) {
        // this is root
        *root = NULL;
      } else if (this == left_->child_) {
        // this is left-most child
        left_->child_ = right_;
        if(right_ != NULL)
          right_->left_ = left_;
      } else if (right_ == NULL) {
        // this is right-most child
        left_->right_ = NULL;
      } else {
        // this is middle child
        left_->right_ = right_;
        right_->left_ = left_;
      }
      left_ = right_ = NULL;
    }


    bool is_root() const {
      return left_ == right_ == 0;
    }

    bool has_parent() const {
      return left_->child_ == this;
    }

    bool is_leaf() const {
      return right_ == 0;
    }

    V const& element() const {
      return value_;
    }

    V& element() {
      return value_;
    }

    self_t* left() const {
      return left_;
    }

    self_t*& left() {
      return left_;
    }

    self_t* right() const {
      return right_;
    }

    self_t*& right() {
      return right_;
    }

    self_t* parent() const {
      return left_;
    }

    self_t*& parent() {
      return left_;
    }
  };
}

#endif
