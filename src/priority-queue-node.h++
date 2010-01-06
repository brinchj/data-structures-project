/*
  A heap node used as a base for various specialized heap nodes

  Authors: Asger Bruun, Jyrki Katajainen © 2009
*/

#ifndef __CPHSTL_HEAP_NODE__
#define __CPHSTL_HEAP_NODE__

#include "assert.h++"
#include <cstddef> // std::size_t
#include <iostream>
#include <list>

namespace cphstl {

  template <typename V, typename A>
  class heap_node {
  public:

    typedef V value_type;
    typedef A allocator_type;
    typedef std::size_t size_type;
    typedef heap_node<V, A> self_t;

    self_t* left_;
    self_t* right_;
    self_t* child_;
    V value_;


  private:

    heap_node();
    heap_node(heap_node const&);
    heap_node& operator=(heap_node const&);

  public:

    heap_node(V const& v, A const&)
      : left_(NULL), right_(NULL), child_(NULL), value_(v) {
    }

    static size_type footprint() {
      return sizeof(self_t);
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
