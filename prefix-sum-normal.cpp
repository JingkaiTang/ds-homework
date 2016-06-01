#include <iostream>
#include <algorithm>
#include <numeric>
#include <functional>

using namespace std;

int main(int argc, char **argv) {
  auto x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int *y = new int[x.size()];

  // for(int i = 0; i < x.size(); i++) {
  //   y[i] = accumulate(x.begin(), x.begin()+i+1, 0, plus<int>());
  // }
  y[0] = *x.begin();
  for(int i = 1; i < x.size(); i++) {
    y[i] = y[i-1] + *(x.begin()+i);
  }

  cout << "x[] => ";
  for_each(x.begin(), x.end(), [](auto i) {
    cout << i << ' ';
  });
  cout << endl;

  cout << "y[] => ";
  for_each(y, y+x.size(), [](auto i) {
    cout << i << ' ';
  });
  cout << endl;

  delete[] y;
  return 0;
}
