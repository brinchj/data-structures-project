/*  
  A weak-heap node

  Author: Jyrki Katajainen © 2009
*/

#ifndef __CPHSTL_WEAK_HEAP_NODE__
#define __CPHSTL_WEAK_HEAP_NODE__

#include "heap-node.h++"

namespace cphstl {

  template <typename V, typename A>
  class pair_heap_node
    : public heap_node<V, A, pair_heap_node<V, A> > {

  private:
    
    pair_heap_node();
    pair_heap_node(pair_heap_node const&);
    pair_heap_node& operator=(pair_heap_node const&);

  public:

    typedef V value_type;
    typedef A allocator_type;
    typedef pair_heap_node<V, A> N;

    pair_heap_node(V const& v, A const& a)
      : heap_node<V, A, N>(v, a) {
    }

#ifdef DEBUG

    template <typename C, typename M>
    bool is_valid(C const& comparator, M const& mark_store) const {
      N const* t = (*this).down_cast(this);
      bool valid = true;
      if ((*t).parent() != 0) {
        valid &= t -> parent() -> left() == t || 
          t -> parent() -> right() == t;
      }
      if ((*t).left() != 0) {
        valid &= t -> left() -> parent() == t;
      }
      if ((*t).right() != 0) {
        valid &= t -> right() -> parent() == t;
      }
      if (! (*t).is_root() && ! mark_store.is_marked(t)) {
        valid &= ! comparator((*t).distinguished_ancestor() -> element(), (*t).element());
      }
      return valid;
    }      

#endif

  };  
} 

#endif
