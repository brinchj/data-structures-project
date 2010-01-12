/*
  A heap node used as a base for various specialized heap nodes

  Authors: Asger Bruun, Jyrki Katajainen © 2009
*/

#ifndef __CPHSTL_PAIRING_HEAP_NODE__
#define __CPHSTL_PAIRING_HEAP_NODE__

#include <cstddef> // std::size_t
#include <iostream>
#include <list>

#include "doubly-linked-list.h++"

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
    typedef std::size_t size_type;
    typedef pairing_heap_node<V, A, C> self_t;

    self_t* left_;
    self_t* right_;
    self_t* child_;
    int color_;
    V value_;
    A allocator_;
    C comparator_;

    list_node<self_t*> *node_in_list_;

  private:

    pairing_heap_node(pairing_heap_node const&);
    pairing_heap_node& operator=(pairing_heap_node const&);

  public:

    pairing_heap_node()
      : left_(NULL), right_(NULL), child_(NULL), color_(0), node_in_list_(NULL)
    {
          allocator_  = A();
          comparator_ = C();
    }

    pairing_heap_node(V const& value,
                      std::allocator<pairing_heap_node<V,A,C> > const& a)
      : left_(NULL), right_(NULL), child_(NULL), color_(0), node_in_list_(NULL)
    {
          allocator_  = A();
          value_ = value;
          comparator_ = C();
    }

    pairing_heap_node(V const& v, A const& a)
      : left_(NULL), right_(NULL), child_(NULL), color_(0),
        value_(v), allocator_(a), node_in_list_(NULL) {
          comparator_ = C();
    }

    ~pairing_heap_node() {
      left_ = right_ = child_ = NULL;
      node_in_list_ = NULL;
      //printf(">> destroying %p\n", this);
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


    self_t* meld(self_t* b, bool this_is_smallest = false) {
      // make sure we are smallest
      self_t* a = this;
      if(!this_is_smallest) {
        if(comparator_(value_, b->element())){
          self_t* tmp = this;
          a = b;
          b = tmp;
        }
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


    void list_cut(self_t **list, self_t **list_end, size_t *list_size) {
      // cut element from double-linked list
      if(left_ == NULL) {
        // this is first element
        *list = right_;
        if(right_ != NULL)
          right_->left_ = NULL;
      } else if (right_ == NULL) {
        // this is last element
        left_->right_ = NULL;
        *list_end = left_;
      } else {
        // this is in between two elements
        left_->right_ = right_;
        right_->left_ = left_;
      }
      left_ = right_ = NULL;
      if(list_size != NULL) {
        (*list_size) -= 1;
      }
    }

    void list_add(self_t **list, self_t **list_end, size_t *list_size) {
      if((*list) == NULL) {
        *list = *list_end = this;
        this->left_ = this->right_ = NULL;
      } else {
        (*list_end)->right_ = this;
        this->left_  = *list_end;
        this->right_ = NULL;
        *list_end = this;
      }
      if(list_size != NULL) {
        (*list_size) += 1;
      }
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


    void replace_with(self_t *other) {
      // insert other in this's place
      if(this->left_->child_ && this->left_->child_==this) {
        // this is left-most child (left is parent)
        this->left_->child_ = other;
        other->left_ = this->left_;
        if(this->right_ != NULL) {
          this->right_->left_ = other;
        }
        other->right_ = this->right_;
      } else if (this->right_ == NULL) {
        // this is right-most child
        this->left_->right_ = other;
        other->left_ = this->left_;
        other->right_ = NULL;
      } else {
        // this is somewhere inside child list
        this->left_->right_ = other;
        this->right_->left_ = other;
        other->left_ = this->left_;
        other->right_ = this->right_;
      }
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

    void swap(self_t * other) {
      self_t* left = other->left_;
      self_t* right = other->right_;
      self_t* child = other->child_;
      int color = other->color_;
      other->left_ = left_;
      other->right_ = right_;
      other->child_ = child_;
      other->color_ = color_;
      left_ = left;
      right_ = right;
      child_ = child;
      color_ = color;
    }

  };
}

#endif
