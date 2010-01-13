#include <algorithm>
#include <ctime>
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
 
  std::clock_t start = std::clock();
  if (start == std::clock_t(-1)) {
    std::cerr << "Experiment failed: no clock\n";
    std::exit(1);
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


  stop = std::clock();
  if (stop == std::clock_t(-1)) {
    std::cerr << "Experiment failed: clock overflow\n";
    std::exit(2);
  }

  double running_time = double(stop - start)/double(CLOCKS_PER_SEC);
  running_time -= dual_time;
  double time_per_run = running_time / double(repetitions);
  double time_per_operation = time_per_run / double(number_of_elements*repetitions);
  std::cout << number_of_elements*repetitions << " " << time_per_operation << "\n";

  return 0;
}
