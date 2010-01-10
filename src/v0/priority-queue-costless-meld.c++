#include "priority-queue-pair.c++"

#include <stdio.h>
#include <vector>
#include <math.h>

#include "doubly-linked-list.h++"


#define DEBUG

#ifndef PRIORITY_QUEUE_COSTLESS_MELD
#define PRIORITY_QUEUE_COSTLESS_MELD


namespace cphstl {
  template <
    typename V,
    typename C = std::less<V>,
    typename A = std::allocator<V>,
    typename E = heap_node<V, A>,
    typename P = perfect_component<E>
    >
  class node_compare {
  public:
    node_compare(C const& c) : comparator(c) {}
    bool operator()(E* a, E* b) {
      return comparator(a->element(), b->element());
    }
  protected:
    C comparator;
  };

  template <
    typename V,
    typename C = std::less<V>,
    typename A = std::allocator<V>,
    typename E = heap_node<V, A>,
    typename P = perfect_component<E>
    >
  class priority_queue_costless_meld :
    public priority_queue_pair<V,C,A,E,P> {
  public:

    typedef priority_queue<V,C,A,E,P> PQ;
    typedef priority_queue_pair<V,C,A,E,P> PQP;


    // types
    typedef std::size_t size_type;

    priority_queue_costless_meld(C const& c, A const& a)
      : comparator(c), allocator(a), increased_list_(NULL) {
      PQ::top_ = NULL;
      PQ::size_ = 0;
    }

    ~priority_queue_costless_meld() {
      // precondition: The data structure contains no elements
    }


    // Modifiers

    /* Insert element */
    void insert(E* p) {
      //printf("insert: %i\n", p->element());
      assert(p != NULL);
      // heap is empty
      if(PQ::size_ == 0) {
        PQ::top_ = p;
        PQ::size_ = 1;
        return;
      }
      PQ::top_ = PQP::meld_nodes(PQ::top_, p);
      if(PQ::min_ == NULL ||
         comparator(PQ::min_->element(), p->element())) {
        PQ::min_ = p;
      }
      PQ::size_ += 1;

      //printf("insert: is_valid()\n");
      PQP::is_valid();

      return;
    }

    /* Increase value of element */
    void increase(E* p, V const& v) {
      //printf("INCR: %i -> %i\n", p->element(), v);

      assert(p != NULL);
      assert(comparator(p->element(), v));

      p->value_ = v;
      if(PQ::min_ == NULL ||
         comparator(PQ::min_->element(), p->element())) {
        PQ::min_ = p;
      }

      if(p != PQ::top_ && p->color_ == 0) {
        // insert into list of increased nodes
        p->color_ = 1;
        increased_list_.push_front(p);
      }

      //printf("increase: is_valid()\n");
      //PQP::is_valid();
    }

    E* extract() {
      if(increased_list_.size() > 0)
        cleanup();
      //printf("extract->cleanup: is_valid()\n");
      //PQP::is_valid();

      //printf("extract: is_valid()\n");
      PQ::min_ = PQP::extract();
      return PQ::min_;
    }

    void meld(priority_queue_costless_meld& other) {
      PQ::size_ += other.PQ::size_;
      other.cleanup();
      PQ::top_ = PQP::meld_nodes(PQ::top_, other.PQ::top_);
      PQ::min_ = PQ::top_;
    }

    void swap(priority_queue_costless_meld& other) {
      E*        newtop  = other.top_;
      size_type newsize = other.size_;
      other.top_  = newtop;
      other.size_ = newsize;
      PQ::top_  = newtop;
      PQ::size_ = newsize;
    }

    void cleanup() {

      // size of list to sort
      const int N = ceil(log(PQ::size_));

      // comparator
      node_compare<V,C,A,E,P>* ncmp =
        new node_compare<V,C,A,E,P>(comparator);
      doubly_linked_list<E*> list;

      list_node<E*> *node;
      list_node<E*> *sentinel = increased_list_.end();
      while( (node = increased_list_.begin()) != sentinel) {
        increased_list_.erase(node);
        node->content()->color_ = 0;

        E* p = node->content();
        assert(p != NULL);

        // cut out left child
        if(p->child_ != NULL) {
          //printf("has left child\n");
          E* myleft = p->child_;
          p->child_ = myleft->right_;
          if(p->child_) {
            p->child_->left_ = p;
          }
          // insert myleft in p's place
          if(p->left_->child_ && p->left_->child_==p) {
            // p is left-most child (left is parent)
            //printf("left-most\n");
            p->left_->child_ = myleft;
            myleft->left_ = p->left_;
            if(p->right_ != NULL) {
              p->right_->left_ = myleft;
            }
            myleft->right_ = p->right_;
          } else if (p->right_ == NULL) {
            //printf("right-most\n");
            // p is right-most child
            p->left_->right_ = myleft;
            myleft->left_ = p->left_;
            myleft->right_ = NULL;
          } else {
            // p is somewhere inside child list
            //printf("inside\n");
            p->left_->right_ = myleft;
            p->right_->left_ = myleft;
            myleft->left_ = p->left_;
            myleft->right_ = p->right_;
          }
        } else {
          //printf("no child\n");
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
        }
        //printf("cut\n");
        //PQP::is_valid();

        list.push_front(p);
        if(list.size() == N) {
          list.sort(*ncmp);
          list_node<E*> *mynode;
          E* tree = list.begin()->content();
          list.erase(list.begin());
          while( (mynode = list.begin()) != list.end() ) {
            list.erase(mynode);
            tree = meld_nodes(tree, mynode->content());
          }
          PQ::top_ = meld_nodes(PQ::top_, tree);
        }
      }
      if(list.size() > 0) {
        list.sort(*ncmp);
        list_node<E*> *mynode;
        E* tree = list.begin()->content();
        list.erase(list.begin());
        while( (mynode = list.begin()) != list.end() ) {
          list.erase(mynode);
          tree = meld_nodes(tree, mynode->content());
        }
        PQ::top_ = meld_nodes(PQ::top_, tree);
      }
      PQ::min_ = PQ::top_;
    }


    priority_queue_costless_meld() {}


    C comparator;
    A allocator;

    doubly_linked_list<E*> increased_list_;
  };
}
#endif

