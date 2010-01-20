
#ifndef PAIRING_HEAP_POLICY_LAZY_INSERT_INCREASE
#define PAIRING_HEAP_POLICY_LAZY_INSERT_INCREASE

#include <stdio.h>
#include <vector>
#include <math.h>

#include "pairing-heap-node.c++"
#include "doubly-linked-list.h++"


// bit-flags used for tagging
#define NODE_IN_TREE          (1<<0)
#define NODE_IN_INSERT_LIST   (1<<1)
#define NODE_IN_INCREASE_LIST (1<<2)
// used in debug validation
#define NODE_CHECKED          (1<<8)


// bit-flag macros
#define FLAG_HAS(node, flag)     ((node)->color_  &  flag)
#define FLAG_ADD(node, flag)     ((node)->color_ |=  flag)
#define FLAG_REM(node, flag)     ((node)->color_ &= ~flag)


//#define MOVE_INSERTED
#define INSERT_INCREASE

#ifndef DEBUG_VERIFYER
#ifdef DEBUG
#define VERIFY_TREE(root)                       \
  printf("VERIFY AT LINE: %i\n", __LINE__);     \
  is_valid_tree(root, false);                   \
  is_valid_tree(root, true);
#else
#define VERIFY_TREE(root) //
#endif
#endif

namespace cphstl {

#ifndef NODE_COMPARE
#define NODE_COMPARE
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
#endif

template <
  typename V,
  typename C = std::less<V>,
  typename A = std::allocator<V>,
  typename E = pairing_heap_node<V, A, C>
  >
class pairing_heap_policy_lazy_insert_increase {
public:

  // types
  typedef std::size_t size_type;
  typedef pairing_heap_policy_lazy_insert_increase<V,C,A,E> P;

  pairing_heap_policy_lazy_insert_increase(C const& c, A const& a)
    : comparator_(c), allocator_(a), list_insert_(NULL), list_insert_end_(NULL),
      list_increase_(NULL) {
  }

  pairing_heap_policy_lazy_insert_increase()
    : list_insert_(NULL), list_insert_end_(NULL), list_increase_(NULL) {
    comparator_ = C();
    allocator_  = A();
  }


  ~pairing_heap_policy_lazy_insert_increase() {
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
    // heap is empty
    if(*top == NULL) {
      FLAG_ADD(p, NODE_IN_TREE);
      *top = *min = p;
      VERIFY_TREE(*top);
      return;
    }
    assert((*top)->color_ == 1);

#ifdef INSERT_INCREASE
    // add to increase list
    FLAG_ADD(p, NODE_IN_INCREASE_LIST);
    list_increase_.push_front(p);
    p->node_in_list_ = list_increase_.begin();
#else
    // add to insertion list
    FLAG_ADD(p, NODE_IN_INSERT_LIST);
    p->list_add(&list_insert_, &list_insert_end_, NULL);
#endif
    // update minimum
    if(*min == NULL ||
       comparator_((*min)->element(), p->element())) {
      *min = p;
    }
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
    if(p == *top || FLAG_HAS(p, NODE_IN_INCREASE_LIST) ) {
      return;
    }

    // leave newly inserted element in insert list?
    if( FLAG_HAS(p, NODE_IN_INSERT_LIST) ) {
#ifdef MOVE_INSERTED
      FLAG_REM(p, NODE_IN_INSERT_LIST);
      p->list_cut(&list_insert_, &list_insert_end_, NULL);
#else
      return;
#endif
    }

    // insert p into increase list
    FLAG_ADD(p, NODE_IN_INCREASE_LIST);
    list_increase_.push_front(p);
    p->node_in_list_ = list_increase_.begin();
    assert(p->node_in_list_->content() == p);

    VERIFY_TREE(*top);
  }




  E* extract(E **top, E **min, int size) {
    // Precondition: at least two elements in queue, size > 1
    //printf("EXTRACT(MIN)\n");

    if(FLAG_HAS(*min, NODE_IN_INCREASE_LIST)) {
      assert((*min)->node_in_list_ != NULL);
    }
    assert( !FLAG_HAS(*top, NODE_IN_INCREASE_LIST) );

    E* extracted_node = NULL;
    if(FLAG_HAS(*min, NODE_IN_INSERT_LIST) ||
       FLAG_HAS(*min, NODE_IN_INCREASE_LIST)) {
      extracted_node = *min;
    }

    if(list_insert_ != NULL) {
      cleanup_insert(top, min);
    }

    if(list_increase_.size() > 0) {
      cleanup_increase(top, min, size);
    }

    if(extracted_node != NULL && extracted_node != *top) {
      (*top)->swap(extracted_node);
      *top = extracted_node;
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
      assert( !FLAG_HAS(node, NODE_IN_INSERT_LIST) );
      assert( !FLAG_HAS(node, NODE_IN_INCREASE_LIST) );
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
    return extracted_node;
  }


  E* extract(E **top, E **min, E *p) {
    //printf("EXTRACT(E)\n");

    if(FLAG_HAS(p, NODE_IN_INSERT_LIST)) {
      // cut p from insert list
      p->list_cut(&list_insert_, &list_insert_end_, NULL);
    }

    if(FLAG_HAS(p, NODE_IN_INCREASE_LIST)) {
      // cut p from increase list
      list_increase_.erase(p->node_in_list_);
    }
    if(FLAG_HAS(p, NODE_IN_TREE)) {
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
    }

    p->color_ =  0;
    return p;
  }


  void cleanup_insert(E **top, E **min) {
    int count = 0;
    while(list_insert_ != list_insert_end_) {
      //printf("count: %i\n", count);

      // cut first two elements from list
      E* first = list_insert_;
      first->list_cut(&list_insert_, &list_insert_end_, NULL);
      assert(list_insert_ != first);

      E* second = list_insert_;
      second->list_cut(&list_insert_, &list_insert_end_, NULL);
      assert(list_insert_ != second);

      // update colors if set
      if(FLAG_HAS(first, NODE_IN_INSERT_LIST)) {
        FLAG_REM(first, NODE_IN_INSERT_LIST);
        FLAG_ADD(first, NODE_IN_TREE);
        count += 1;
      }
      if(FLAG_HAS(second, NODE_IN_INSERT_LIST)) {
        FLAG_REM(second, NODE_IN_INSERT_LIST);
        FLAG_ADD(second, NODE_IN_TREE);
        count += 1;
      }

      // meld first two nodes
      E* node = first->meld( second );
      assert(node->child_->left_ == node);
      // insert new node as last in list
      node->list_add(&list_insert_, &list_insert_end_, NULL);
      assert(list_insert_ == node || node->left_ != NULL);
    }

    if(list_insert_ != NULL) {
      FLAG_REM(list_insert_, NODE_IN_INSERT_LIST);
      FLAG_ADD(list_insert_, NODE_IN_TREE);

      *top = (*top)->meld( list_insert_ );
      assert((*top)->child_->left_ == *top);
      list_insert_ = list_insert_end_ = NULL;
    }
  }


  void cleanup_increase(E **top, E **min, int size) {
    // size of list to sort
    const size_t logN = ceil(log(size));

    // comparator
    node_compare<V,C,A,E>* ncmp = new node_compare<V,C,A,E>(comparator_);
    doubly_linked_list<E*> *sort_list = new doubly_linked_list<E*>();

    list_node<E*> *node;
    while(list_increase_.size() > 0) {
      node = list_increase_.begin();
      E* p = node->content();

      FLAG_REM(p, NODE_IN_INCREASE_LIST);
      list_increase_.erase(node);
      p->node_in_list_ = NULL;

      assert(p != NULL);

      if( FLAG_HAS(p, NODE_IN_TREE) ) {
        if(p->child_ != NULL) {
          // cut out left child
          E* myleft = p->child_;
          p->child_ = myleft->right_;
          if(p->child_) {
            p->child_->left_ = p;
          }
          // insert myleft in p's place
          p->replace_with(myleft);
        } else {
          // no child, just cut
          p->tree_cut(top);
        }
      }

      // set in tree and add to sort-list
      FLAG_ADD(p, NODE_IN_TREE);
      assert(FLAG_HAS(p, NODE_IN_TREE));
      sort_list->push_front(p);

      VERIFY_TREE(*top);

      // sort and reinsert when threshold is reached
      if(sort_list->size() == logN) {
        cleanup_increase_sort(top, min, logN, sort_list, ncmp);
        VERIFY_TREE(*top);
      }
    }

    VERIFY_TREE(*top);

    // sort and reinsert last elements
    if(sort_list->size() > 0) {
      cleanup_increase_sort(top, min, sort_list->size(), sort_list, ncmp);
    }
  }

  void cleanup_increase_sort(E** top, E** min, int size,
                             doubly_linked_list<E*>* list,
                             node_compare<V,C,A,E>* cmp)
  {
    if(size > 0) {
      // sort list
      list->sort(*cmp);
      // reinsert elements
      list_node<E*> *mynode;
      E* tree = list->begin()->content();
      list->erase(list->begin());
      while( list->size() ) {
        mynode = list->begin();
        tree = (mynode->content())->meld(tree, true);
        list->erase(mynode);
      }
      *top = (*top)->meld( tree );
    }
    *min = *top;
  }



  void meld(E **top, E **min, pairing_heap_framework<V,P,C,A,E> &other) {
  }



  int is_valid_tree(E *root, bool undo=false) {
    if(root == NULL) {
      return 0;
    }

    assert(FLAG_HAS(root, NODE_IN_TREE));

    E* prev = root;
    E* node = root->child_;

    int count = 0;
    while(node != NULL) {
      assert(node->left_ == prev);
      if(undo) {
        assert( FLAG_HAS(node, NODE_CHECKED) );
        FLAG_REM(node, NODE_CHECKED);
      } else {
        assert( !FLAG_HAS(node, NODE_CHECKED) );
        FLAG_ADD(node, NODE_CHECKED);
      }
      assert(FLAG_HAS(node, NODE_IN_TREE));
      assert(root->value_ >= node->value_);

      count += 1 + is_valid_tree(node, undo);

      prev = node;
      node = node->right_;
    }
    return count;
  }


  int is_valid() {
    int count = 0;
    E* node = list_insert_;
    while(node != NULL) {
      count += 1;
      node = node->right_;
    }
    //printf("first-count: %i\n", count);
    list_node<E*> *node2 = list_increase_.begin();
    while(node2 != list_increase_.end()) {
      if( !FLAG_HAS(node2->content(), NODE_IN_TREE) ) {
        //printf("%p -> %p\n", node2->content(), node2);
        count += 1;
      }
      node2 = node2->successor();
    }
    //printf("total-count: %i (%i)\n", count, list_increase_.size());
    return count;
  }




private:
  C comparator_;
  A allocator_;

  E* list_insert_;
  E* list_insert_end_;
  doubly_linked_list<E*> list_increase_;

};
}
#endif

