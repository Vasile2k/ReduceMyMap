#include "Servant.hpp"
#include <iostream>
#include <string>
#include "mpi.h"
#include "Common.hpp"

Servant::Servant(int workerId) : workerId(workerId) {
	std::cout << "Worker " << workerId << " created" << std::endl;
}

Servant::~Servant() {

}

void Servant::run() {
	bool working = true;
	while (working) {
		// Tell the boss I'm free
		notifyMaster();

		std::string nextJob = getNextTask();
		char jobType = nextJob[0];

		switch (jobType) {
			case PACKET_SUICIDE: {
				working = false;
				break;
			}
			case PACKET_MAP_TO_WORDS: {
				const char* filename = nextJob.c_str() + 1;
				std::cout << "Worker " << workerId << " maps file " << filename << std::endl;
				break;
			}
			case PACKET_REDUCE_TO_LETTER: {
				const char* letterAddr = nextJob.c_str() + 1;
				char letter = *letterAddr;
				std::cout << "Worker " << workerId << " reduces file with letter " << letter << std::endl;
				break;
			}
		}
	}
	std::cout << "Worker " << workerId << " ended life." << std::endl;
}

void Servant::notifyMaster() {
	char packet[] = { PACKET_WORKER_FREE };
	MPI_Send(packet, 1, MPI_CHAR, MASTER_ID, 0, MPI_COMM_WORLD);
}

std::string Servant::getNextTask() {
	char message[256];
	MPI_Status status;
	MPI_Recv(message, 256, MPI_CHAR, MASTER_ID, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	// Fail-safe
	message[255] = 0;
	return std::string(message);
}
