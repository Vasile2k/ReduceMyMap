#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include "mpi.h"
#include "Common.hpp"
#include "Master.hpp"
#include "Servant.hpp"

/*
 * Parameters are -i for input path and -o for output path
 * Both are required
 */
int main(int argc, char* argv[]) {
	int rank;
	int size;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// Nice to run with 9 processes: 1 master and 8 slaves
	if (rank == MASTER_ID) {
		Master master(argc, argv, size - 1);
		master.run();
	} else {
		Servant servant(rank);
		servant.run();
	}

	MPI_Finalize();
	return 0;
}
