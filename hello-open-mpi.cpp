#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char **argv) {
  MPI::Init(argc, argv);
  int l_proc = MPI::COMM_WORLD.Get_size();
  int l_rank = MPI::COMM_WORLD.Get_rank();

  cout << "Total Rank: " << l_proc << endl
    << "This Rank: " << l_rank << endl;

  MPI::Finalize();

  return 0;
}
