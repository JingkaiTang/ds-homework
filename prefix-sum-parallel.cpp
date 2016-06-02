#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <mpi.h>

using namespace std;

int main(int argc, char **argv) {
  MPI::Init();
  int size = MPI::COMM_WORLD.Get_size();
  int rank = MPI::COMM_WORLD.Get_rank();

  if (rank == 0) {
    unsigned long seed = std::chrono::system_clock::now().time_since_epoch().count();
    minstd_rand0 generator(seed);
    auto get_rand = [&generator]() -> int {
      return generator() % 100;
    };

    vector<int> arr;
    for (int i = 0; i < size; i++) {
      arr.push_back(get_rand());
    }

    auto print_arr = [&arr]() {
      for_each(arr.begin(), arr.end(), [](auto el) {
        cout << el << ' ';
      });
      cout << endl;
    };

    print_arr();

    for (int i = 1; i < size; i++) {
      MPI::COMM_WORLD.Send(&arr[i], 1, MPI::INT, i, 0);
    }

    for (int step = 1; step < size; step <<= 1) {
      MPI::COMM_WORLD.Send(&arr[0], 1, MPI::INT, step, 0);
    }

    for (int i = 1; i < size; i++) {
      MPI::COMM_WORLD.Recv(&arr[i], 1, MPI::INT, i, 0);
    }
    print_arr();
  } else {
    int sum = 0;
    MPI::COMM_WORLD.Recv(&sum, 1, MPI::INT, 0, 0);

    int tmp = 0;

    for (int step = 1; step < size; step <<= 1) {
      if (rank < size-step) {
        MPI::COMM_WORLD.Send(&sum, 1, MPI::INT, rank+step, 0);
      }
      if (rank >= step) {
        MPI::COMM_WORLD.Recv(&tmp, 1, MPI::INT, rank-step, 0);
        sum += tmp;
      }
    }

    MPI::COMM_WORLD.Send(&sum, 1, MPI::INT, 0, 0);
  }

  MPI::Finalize();

  return 0;
}
