#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include "mpi.h"

int main(int argc, char* argv[]) {
	int rank;
	int size;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// Nice to run with 9 processes: 1 master and 8 slaves

	MPI_Finalize();
	return 0;
}
