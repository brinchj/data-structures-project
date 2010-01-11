#include <algorithm>
#include <ctime>
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
 
  std::clock_t start = std::clock();
  if (start == std::clock_t(-1)) {
    std::cerr << "Experiment failed: no clock\n";
    std::exit(1);
  }

  for (int k = 0; k != repetitions; ++k) {
    for (int i = 0; i != number_of_elements; ++i) {
      (void) q.push(a[i]);
    }
  }

  std::clock_t stop = std::clock();
  if (stop == std::clock_t(-1)) {
    std::cerr << "Experiment failed: clock overflow\n";
    std::exit(2);
  }

  double dual_time = double(stop - start)/double(CLOCKS_PER_SEC);

  start = std::clock();
  if (start == std::clock_t(-1)) {
    std::cerr << "Experiment failed: no clock\n";
    std::exit(1);
  }

  for (int k = 0; k != repetitions; ++k) {
    for (int i = 0; i != number_of_elements; ++i) {
      (void) q.push(a[i]);
    }
    for (int i = 0; i != number_of_elements; ++i) {
      q.pop();
    }
  }

  stop = std::clock();
  if (stop == std::clock_t(-1)) {
    std::cerr << "Experiment failed: clock overflow\n";
    std::exit(2);
  }

  double running_time = double(stop - start)/double(CLOCKS_PER_SEC);
  running_time -= dual_time;
  double time_per_run = running_time / double(repetitions);
  double time_per_operation = time_per_run / double(number_of_elements);
  std::cout << number_of_elements << " " << time_per_operation << "\n";

  return 0;
}
