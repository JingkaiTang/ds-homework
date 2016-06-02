#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <cstdlib>

#define ARRAY_LEN 10

using namespace std;

int main(int argc, char **argv) {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  minstd_rand0 generator(seed);
  auto get_rand = [&generator]() -> int {
    return generator() % 100;
  };

  int len = argc > 1 ? atoi(argv[1]) : ARRAY_LEN;
  vector<int> arr;
  for(int i = 0; i < len; i++) {
    arr.push_back(get_rand());
  }

  auto print_arr = [&arr]() {
    for_each(arr.begin(), arr.end(), [](auto el) {
      cout << el << ' ';
    });
    cout << endl;
  };

  print_arr();

  auto normal_impl = [&arr]() {
    for(auto it = arr.begin()+1; it != arr.end(); ++it) {
      *it += *(it-1);
    }
  };
  normal_impl();
  print_arr();

  auto cpp_impl = [&arr]() {
    partial_sum(arr.begin(), arr.end(), arr.begin());
  };
  cpp_impl();
  print_arr();

  return 0;
}
