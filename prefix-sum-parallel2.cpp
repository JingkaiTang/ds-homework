#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <cstdlib>
#include <mpi.h>

#define ARRAY_LEN 10

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

    int len = argc > 1 ? atoi(argv[1]) : ARRAY_LEN;
    vector<int> arr;
    for (int i = 0; i < len; i++) {
      arr.push_back(get_rand());
    }

    auto print_arr = [&arr]() {
      for_each(arr.begin(), arr.end(), [](auto el) {
        cout << el << ' ';
      });
      cout << endl;
    };

    print_arr();

    int cap = (len+size-1)/size;
    vector<int> caps;
    int remain = len;
    for (int i = 0; i < size; i++) {
      if (remain >= cap) {
        caps.push_back(cap);
        remain -= cap;
      } else if (remain > 0) {
        caps.push_back(remain);
        remain = 0;
      } else {
        caps.push_back(0);
      }
    }

    int index = caps[0];
    for (int i = 1; i < size; i++) {
      MPI::COMM_WORLD.Send(&caps[i], 1, MPI::INT, i, 0);
      if (caps[i] > 0) {
        MPI::COMM_WORLD.Send(&arr[index], caps[i], MPI::INT, i, 0);
        index += caps[i];
      }
    }

    partial_sum(arr.begin(), arr.begin()+caps[0], arr.begin());
    if (size > 1) {
      MPI::COMM_WORLD.Send(&arr[caps[0]-1], 1, MPI::INT, 1, 0);
    }

    index = caps[0];
    for (int i = 1; i < size; i++) {
      if (caps[i] > 0) {
        MPI::COMM_WORLD.Recv(&arr[index], caps[i], MPI::INT, i, 0);
        index += caps[i];
      }
    }
    print_arr();
  } else {
    int cap = 0;
    MPI::COMM_WORLD.Recv(&cap, 1, MPI::INT, 0, 0);

    if (cap > 0) {
      int *buf = new int[cap];
      MPI::COMM_WORLD.Recv(buf, cap, MPI::INT, 0, 0);
      partial_sum(buf, buf+cap, buf);
      int update = 0;
      MPI::COMM_WORLD.Recv(&update, 1, MPI::INT, rank-1, 0);
      buf[cap-1] += update;
      if (rank < size-1) {
        MPI::COMM_WORLD.Send(&buf[cap-1], 1, MPI::INT, rank+1, 0);
      }
      for(auto it = buf; it != buf+cap-1; ++it) {
        *it += update;
      }
      MPI::COMM_WORLD.Send(buf, cap, MPI::INT, 0, 0);
      delete[] buf;
    }
  }

  MPI::Finalize();

  return 0;
}
