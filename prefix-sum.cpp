#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <vector>

using namespace std;

int main(int argc, char **argv) {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  minstd_rand0 generator(seed);
  auto get_rand = [&generator]() {
    return generator() % 100;
  };

  int len = 10;
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

  for(auto it = arr.begin()+1; it != arr.end(); ++it) {
    *it += *(it-1);
  }
  print_arr();

  return 0;
}
