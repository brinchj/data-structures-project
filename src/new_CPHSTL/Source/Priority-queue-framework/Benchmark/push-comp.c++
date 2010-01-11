#include <algorithm>
#include <ctime>
#include <iostream>

#ifndef NUMBER 
#define NUMBER 1000000
#endif

#include "data-structure.i++" // Q and V come from here

int main() {
  int const number_of_elements = NUMBER;
  int const repetitions = 20;

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
    for (int i = 0; i != number_of_elements; ++i) {
      (void) q.push(a[i]);
    }
  }

  double comp_count = double(comps);
  double comp_per_run = comp_count / double(repetitions);
  double comp_per_operation = comp_per_run / double(number_of_elements);
  std::cout << number_of_elements << " " << comp_per_operation << "\n";

  return 0;
}
