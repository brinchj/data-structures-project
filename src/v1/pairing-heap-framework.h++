/*
  A pairing-heap framework that can be used to generate a realizator
  for a meldable priority queue. The efficiency of insert, extract and
  increase operations depends on the policies relied on (P).

*/

#ifndef __CPHSTL_PAIRING_HEAP_FRAMEWORK__
#define __CPHSTL_PAIRING_HEAP_FRAMEWORK__

#include <cstddef> // std::size_t and std::ptrdiff_t
#include <functional> // std::less
#include <memory> // std::allocator
#include "weak-heap-node.h++"

namespace cphstl {

  template <
    typename V,
    typename P,
    typename C  = std::less<V>,
    typename A  = std::allocator<V>,
    typename E  = weak_heap_node<V, A>
  >
  class pairing_heap_framework {
  public:

    // types

    typedef V value_type;
    typedef C comparator_type;
    typedef A allocator_type;
    typedef E encapsulator_type;
    typedef P policy_type;
    typedef std::size_t size_type;

    // structors

    explicit pairing_heap_framework(C const& = C(), A const& = A(),
                                    P const& = P());
    ~pairing_heap_framework();

    // iterators

    E* begin() const;
    E* end() const;

    // accessors

    A get_allocator() const;
    C get_comparator() const;
    size_type size() const;
    size_type max_size() const;
    E* top() const;

    // modifiers

    void insert(E*);
    E* extract();
    void extract(E*);
    void increase(E*, V const&);
    void meld(pairing_heap_framework&);
    void swap(pairing_heap_framework&);

  protected:

    C comparator;
    A allocator;
    P policy;
    E* top_;
    E* min_;
    size_type size_;

  private:

    pairing_heap_framework(pairing_heap_framework const&);
    pairing_heap_framework& operator=(pairing_heap_framework const&);
  };

}

#include "pairing-heap-framework.i++"
#endif