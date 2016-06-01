#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char **argv) {
  MPI::Init();
  int l_proc = MPI::COMM_WORLD.Get_size();
  int l_rank = MPI::COMM_WORLD.Get_rank();

  int a = 2222;

  MPI::COMM_WORLD.Send(&a, 1, MPI_INT, (l_rank+1)%l_proc, 0);

  int b = 0;
  MPI::COMM_WORLD.Recv(&b, 1, MPI_INT, (l_rank-1)%l_proc, 0);
  cout << "In Proc " << l_rank << " " << b << endl;

  MPI::Finalize();

  return 0;
}
