#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>

using namespace std;

int main(int argc, char **argv) {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  minstd_rand0 generator(seed);
  auto get_rand = [&generator]() {
    return generator() % 100;
  };

  int arr_len = 10;
  int *arr = new int[10];
  for(int i = 0; i < arr_len; i++) {
    arr[i] = get_rand();
  }

  auto print_arr = [&arr, &arr_len]() {
    for_each(arr, arr+arr_len, [](auto el) {
      cout << el << ' ';
    });
  };

  cout << "x[] => ";
  print_arr();
  cout << endl;

  for(int i = 1; i < arr_len; i++) {
    arr[i] += arr[i-1];
  }

  cout << "y[] => ";
  print_arr();
  cout << endl;

  return 0;
}
