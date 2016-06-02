#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char **argv) {
  MPI::Init();
  int l_proc = MPI::COMM_WORLD.Get_size();
  int l_rank = MPI::COMM_WORLD.Get_rank();

  MPI::COMM_WORLD.Send(&l_rank, 1, MPI::INT, (l_rank+1)%l_proc, 0);

  int buf = 0;
  MPI::COMM_WORLD.Recv(&buf, 1, MPI::INT, (l_rank-1)%l_proc, 0);
  cout << "I am rank " << l_rank << ", I received a message from rank " << buf << endl;

  MPI::Finalize();

  return 0;
}
