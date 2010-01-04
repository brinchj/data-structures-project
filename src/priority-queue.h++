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
#include "priority-queue-node.h++"
#include "perfect-component.h++"

namespace cphstl {
        template <
                typename V,
                typename C = std::less<V>,
                typename A = std::allocator<V>,
                typename E = heap_node<V, A>,
                typename P = perfect_component<E>
                >
        class priority_queue {
        public:

                // types
                typedef std::size_t size_type;

                // structors

                explicit priority_queue(C const& = C(), A const& = A()) {}
                ~priority_queue() {}

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
                void meld(priority_queue&);
                void swap(priority_queue&);


        private:

                C comparator;
                A allocator;
                E* top_;
                size_type size_;
				E* auxlist;
				E* auxlist_last;

        };

}

#endif
