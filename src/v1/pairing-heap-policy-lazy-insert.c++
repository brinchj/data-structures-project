#ifndef PAIRING_HEAP_POLICY_LAZY_INSERT
#define PAIRING_HEAP_POLICY_LAZY_INSERT

// includes
#include <stdio.h>
#include <vector>
#include "pairing-heap-node.c++"


// bit-flags used for tagging
#define NODE_IN_TREE          (1<<0)
#define NODE_IN_INSERT_LIST   (1<<1)
// used in debug validation
#define NODE_CHECKED          (1<<8)

// bit-flag macros
#define FLAG_HAS(node, flag)     ((node)->color_  &  flag)
#define FLAG_ADD(node, flag)     ((node)->color_ |=  flag)
#define FLAG_REM(node, flag)     ((node)->color_ &= ~flag)



namespace cphstl {
  template <
    typename V,
    typename C = std::less<V>,
    typename A = std::allocator<V>,
    typename E = pairing_heap_node<V, A, C>
    >
  class pairing_heap_policy_lazy_insert {
  public:

    // types
    typedef std::size_t size_type;
    typedef pairing_heap_policy_lazy_insert<V,C,A,E> P;

    pairing_heap_policy_lazy_insert(C const& c, A const& a)
      : comparator_(c), allocator_(a) {
      list_ = list_end_ = NULL;
    }

    pairing_heap_policy_lazy_insert() {
      comparator_ = C();
      allocator_  = A();
      list_ = list_end_ = NULL;
    }


    ~pairing_heap_policy_lazy_insert() {
      // precondition: The data structure contains no elements
    }

    E *begin() const {
      return NULL;
    }

    E *end() const {
      return NULL;
    }



    /* Insert element */
    void insert(E **top, E **min, E* p) {
      // heap is empty
      if(*top == NULL) {
        FLAG_ADD(p, NODE_IN_TREE);
        *top = *min = p;
        return;
      }

      // add to list
      FLAG_ADD(p, NODE_IN_INSERT_LIST);
      p->list_add(&list_, &list_end_, NULL);

      // update minimum
      if(*min == NULL ||
         comparator_((*min)->element(), p->element())) {
        *min = p;
      }
    }

    /* Increase value of element */
    void increase(E **top, E **min, E* p, V const& v) {
      p->value_ = v;

      // if p is top we're done
      if(p == *top || FLAG_HAS(p, NODE_IN_INSERT_LIST)) {
        if(*min == NULL ||
           comparator_((*min)->element(), p->element())) {
          *min = p;
        }
        return;
      }

      // remove p from child-list
      p->tree_cut(top);

      // reinsert p
      p->left_ = p->right_ = NULL;
      *top = *min = (*top)->meld( p );
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
          assert(FLAG_HAS(node, NODE_CHECKED));
          FLAG_REM(node, NODE_CHECKED);
        } else {
          assert( !FLAG_HAS(node, NODE_CHECKED) );
          FLAG_ADD(node, NODE_CHECKED);
        }

        count += 1 + is_valid_tree(node, undo);

        prev = node;
        node = node->right_;
      }
      return count;
    }




    E* extract(E **top, E **min, int size) {
      // Precondition: at least two elements in queue, size > 1
      // process waiting nodes

      bool min_from_list = FLAG_HAS(*min, NODE_IN_INSERT_LIST);
      if(min_from_list) {
        // cut minimum from list
        (*min)->list_cut(&list_, &list_end_, NULL);
        FLAG_REM(*min, NODE_IN_INSERT_LIST);
      }

      while(list_ != list_end_) {
        // cut first two elements from list
        E* first = list_;
        first->list_cut(&list_, &list_end_, NULL);
        assert(list_ != first);

        E* second = list_;
        second->list_cut(&list_, &list_end_, NULL);
        assert(list_ != second);

        // update colors
        FLAG_REM(first, NODE_IN_INSERT_LIST);
        FLAG_REM(second, NODE_IN_INSERT_LIST);

        // meld first two nodes
        E* node = first->meld( second );
        assert(node->child_->left_ == node);
        // insert new node as last in list
        node->list_add(&list_, &list_end_, NULL);
        assert(list_ == node || node->left_ != NULL);
      }

      if(list_ != NULL) {
        FLAG_REM(list_, NODE_IN_INSERT_LIST);
        *top = (*top)->meld( list_ );
        assert((*top)->child_->left_ == *top);
        list_ = list_end_ = NULL;
      }

      // if minimum came from lazy list, just return it
      if(min_from_list) {
        E* extracted_node = *min;
        *min = *top;
        return extracted_node;
      }

      // pick top for extraction
      E* extracted_node = *top;
      assert(*min == *top);

      // iterate through children
      if((*top)->child_ != NULL) {
        E* list = NULL;
        E* node = (*top)->child_;
        while(node != NULL && node->right_ != NULL) {
          E* next = node->right_->right_;
          // meld with first in list
          E* right = node->right_;
          node = node->meld( right );
          // set merged node as first in list
          assert(node->child_->left_ == node);
          node->right_ = list;
          list = node;
          // move to next child
          node = next;
        }

        // prepend last element to list
        if(node != NULL) {
          node->right_ = list;
          list = node;
        }

        // meld elements in list
        *top = list;
        node = list->right_;
        while(node != NULL) {
          E* next = node->right_;
          *top = (*top)->meld( node );
          assert((*top)->child_->left_ == *top);
          node = next;
        }
        (*top)->left_ = (*top)->right_ = NULL;
      }
      else
        {
          *top = NULL;
        }

      *min = *top;
      assert(list_ == NULL && list_end_ == NULL);

      return extracted_node;
    }

    E* extract(E **top, E **min, E *p) {
      //printf("extract(%p) called\n", p);

      if(FLAG_HAS(p, NODE_IN_INSERT_LIST)) {
        // cut p from list
        p->list_cut(&list_, &list_end_, NULL);
        return p;
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


    void meld(E **top, E **min, pairing_heap_framework<V,P,C,A,E> &other) {
      // stubbed
    }


    int is_valid_list(E* root, bool undo) {
      E* node = root;
      assert(node->left_ == NULL);
      if(undo) {
        assert(FLAG_HAS(node, NODE_CHECKED));
        FLAG_REM(node, NODE_CHECKED);
      } else {
        assert(!FLAG_HAS(node, NODE_CHECKED));
        FLAG_ADD(node, NODE_CHECKED);
      }

      size_type count = 1;

      node = node->right_;
      while(node != NULL) {
        assert(node->left_ != NULL);
        assert(node->left_->right_ == node);
        if(undo) {
          assert(FLAG_HAS(node, NODE_CHECKED));
          FLAG_REM(node, NODE_CHECKED);
        } else {
          assert(!FLAG_HAS(node, NODE_CHECKED));
          FLAG_ADD(node, NODE_CHECKED);
        }
        node = node->right_;
        count += 1;
      }
      return count;
    }

    int is_valid() {
      if(list_ == NULL)
        return 0;
      is_valid_list(list_, false);
      return is_valid_list(list_, true);
    }



  private:
    C comparator_;
    A allocator_;

    E *list_, *list_end_;

  };
}
#endif

