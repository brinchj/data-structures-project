#include <algorithm>
#include <cmath>
#include <iostream>

#ifndef NUMBER 
#define NUMBER 1000000
#endif

#include "data-structure.i++" // Q and V come from here

int main() {
  int const number_of_elements = NUMBER / 10;
  int const repetitions = 10;

  Q q;
  V* a = new V[number_of_elements*repetitions];
  V* b = new V[number_of_elements*repetitions];

  for (int i = 0; i < number_of_elements; i++) {
    for (int k = 0; k < repetitions; k++) {
 	  a[i] = (V) i;
 	  b[i] = (V) i*repetitions + k;
	}
  }

  srand(1);
  for (int i = 0; i < number_of_elements; i++) {
    std::swap(b[i], b[rand() % (number_of_elements*repetitions)]);
  }

  double dual_comp = double(comps);
  comps = 0;

  for (int k = 0; k != repetitions; ++k) {
    std::vector<Q::iterator> v;
    for (int i = 0; i != number_of_elements*repetitions; ++i) {
      Q::iterator p = q.push(a[i]);
      v.push_back(p);
    }

	for (int i = 0; i < number_of_elements; i++) {
	  for (int k = 0; k < repetitions; ++k) {
        Q::iterator p = q.top();
        q.increase(v[b[(i*repetitions + k)]], *v[b[(i*repetitions + k)]] + number_of_elements*10);//*q.top() + 1);
	  }
	}
	Q::iterator p = q.top();
	q.pop();
  }

  double comp_count = double(comps);
  comp_count -= dual_comp;
  double comp_per_run = comp_count / double(repetitions);
  double comp_per_operation = comp_per_run / double(number_of_elements*repetitions);
  double factor = comp_per_operation / std::log2(double(number_of_elements*repetitions));
  std::cout << number_of_elements*repetitions << " " << comp_per_operation << " ";
  std::cout << "(" << factor << " lg n)\n";

  return 0;
}
