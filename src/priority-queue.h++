/*
  A priority-queue framework can be used to generate a realizator for
  a meldable priority queue. The efficiency of different operations
  depends on the policies relied on.

  Author: Jyrki Katajainen © 2009
*/

#ifndef __CPHSTL_PRIORITY_QUEUE_FRAMEWORK__
#define __CPHSTL_PRIORITY_QUEUE_FRAMEWORK__

#include <cstddef> // std::size_t and std::ptrdiff_t
#include <functional> // std::less
#include <memory> // std::allocator
#include <vector>
#include "heap-node.h++"
#include "perfect-component.h++"
#include "weak-heap-node.h++"

namespace cphstl {
  template <
    typename V,
    typename C = std::less<V>,
    typename A = std::allocator<V>,
    typename E = weak_heap_node<V, A>,
    typename P = perfect_component<E>
    >
  class priority_queue {
  public:

    // types
    typedef std::size_t size_type;

    // structors

    explicit priority_queue(C const& = C(), A const& = A()) {}
    ~priority_queue() {}


    // accessors


    // Accessors
    void begin() const {
      if(size_ == 0) {
        return NULL;
      }
      return top_;
    }

    void end() const {
      return NULL;
    }

    A* get_allocator() const {
      return allocator;
    }

    C* get_comparator() const {
      return comparator;
    }

    int size() const {
      return size_;
    }

    int max_size() const {
      typename std::vector<int, A>::allocator_type a;
      size_type available_memory = a.max_size() * sizeof(int);
      return available_memory / E::footprint();
    }

    E* find_min() const {
      return min_;
    }

    // modifiers

    void insert(E*);
    E* extract();
    void extract(E*);
    void increase(E*, V const&);
    void meld(priority_queue&);
    void swap(priority_queue&);



  protected:
    C comparator;
    A allocator;

    size_type size_;
    E* auxlist;
    E* auxlist_last;

    E* min_;
    E* top_;
  };

}

#endif
