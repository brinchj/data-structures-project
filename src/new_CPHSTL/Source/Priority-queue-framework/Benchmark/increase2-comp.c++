#include <algorithm>
#include <cmath>
#include <iostream>

#ifndef NUMBER 
#define NUMBER 1000000
#endif

#include "data-structure.i++" // Q and V come from here

int main() {
  int const number_of_elements = NUMBER;
  int const repetitions = 10;

  Q q;
  V* a = new V[number_of_elements];

  for (int i = 0; i < number_of_elements; i++) {
    a[i] = (V) i;
  }

  srand(1);
  for (int i = 0; i < number_of_elements; i++) {
    std::swap(a[i], a[rand() % (number_of_elements)]);
  }
 
  for (int k = 0; k != repetitions; ++k) {
    std::vector<Q::iterator> v;
    for (int i = 0; i != number_of_elements; ++i) {
      Q::iterator p = q.push(a[i]);
      v.push_back(p);
    }
  }

  double dual_comp = double(comps);
  comps = 0;

  for (int k = 0; k != repetitions; ++k) {
    std::vector<Q::iterator> v;
    for (int i = 0; i != number_of_elements; ++i) {
      Q::iterator p = q.push(a[i]);
      v.push_back(p);
    }

    for (int i = 0; i != number_of_elements; ++i) {
      Q::iterator p = q.top();
      q.increase(v[i], *v[i] + number_of_elements);//*q.top() + 1);
    }
    
	for (int i = 0; i != number_of_elements; ++i) {
      Q::iterator p = q.top();
      q.pop();
    }
  }

  double comp_count = double(comps);
  comp_count -= dual_comp;
  double comp_per_run = comp_count / double(repetitions);
  double comp_per_operation = comp_per_run / double(number_of_elements);
  double factor = comp_per_operation / std::log2(double(number_of_elements));
  std::cout << number_of_elements << " " << comp_per_operation << " ";
  std::cout << "(" << factor << " lg n)\n";

  return 0;
}
