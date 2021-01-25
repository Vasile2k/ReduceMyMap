#include <filesystem>
#include <iostream>
#include <string>
#include <queue>
#include <mpi.h>
#include "Master.hpp"
#include "Common.hpp"

Master::Master(int argc, char* argv[], int workerCount) : availableWorkers(), workerCount(workerCount), temporaryDirectory("./temp-dir/") {
	
	bool inputPathSet = false;
	bool outputPathSet = false;

	// argc - 1 because the last argument can't be -i or -o
	for (int i = 0; i < argc - 1; ++i) {
		if (strcmp(argv[i], INPUT_PATH_ARGUMENT) == 0) {
			inputDirectory = argv[i+1];
			inputPathSet = true;
		} else if (strcmp(argv[i], OUTPUT_PATH_ARGUMENT) == 0) {
			outputDirectory = argv[i+1];
			outputPathSet = true;
		}
	}

	// Make sure paths end with slash for easier concatenation with filename
	if (!inputDirectory.back() != '/') {
		inputDirectory += '/';
	}
	if (!outputDirectory.back() != '/') {
		outputDirectory += '/';
	}

	if (inputPathSet) {
		std::cout << "Master: Input path set to " << inputDirectory << std::endl;
	} else {
		std::cout << "Master: Input path not set!" << std::endl;
	}
	if (outputPathSet) {
		std::cout << "Master: Output path set to " << outputDirectory << std::endl;
	} else {
		std::cout << "Master: Output path not set!" << std::endl;
	}

	if (inputPathSet && outputPathSet) {
		initTasks();
	} else {
		std::cout << "Master: Can't continue, path are not both set!" << std::endl;
	}

	std::cout << "Master created" << std::endl;
}

Master::~Master() {

}

void Master::run() {
	bool alive = true;
	while (alive) {
		
		char data[256];

		MPI_Status status;
		MPI_Recv(data, 256, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		if (data[0] == PACKET_WORKER_FREE) {
			availableWorkers.push(status.MPI_SOURCE);
			std::cout << "Master: worker " << status.MPI_SOURCE << " free." << std::endl;
		} else {
			std::cout << "Master received unknown packet! Fuck!" << std::endl;
		}
		
		if (remainingTasks.size() == 0 && availableWorkers.size() == workerCount) {
			// Kill all and end life
			
			std::string killPacket = "";
			killPacket += PACKET_SUICIDE;

			while (!availableWorkers.empty()) {
				int worker = availableWorkers.front();
				sendJob(killPacket, worker);
				availableWorkers.pop();
			}
			alive = false;
		} else {
			if (remainingTasks.size() == 0) {
				// No more tasks, wait for all to finish
				continue;
			}
			std::string& nextTask = remainingTasks.front();

			if (nextTask[0] == PACKET_BARRIER) {
				// Wait for all workers to finish

				if (availableWorkers.size() == workerCount) {
					// Barrier waiting done, now next task
					remainingTasks.pop();
					while (remainingTasks.size() > 0 && availableWorkers.size() > 0) {
						nextTask = remainingTasks.front();
						sendJob(nextTask, availableWorkers.front());
						availableWorkers.pop();
						remainingTasks.pop();
					}
					std::cout << "Master finished waiting at barrier!" << std::endl;
					continue;
				} else {
					// Otherwise do nothing until all of them are free
					continue;
				}
			}
			
			if (availableWorkers.size() == 0) {
				// No workers available? Well...
				std::cout << "Master: Some kind of shit happened! No workers?" << std::endl;
				continue;
			}

			int nextFreeWorker = availableWorkers.front();
			sendJob(nextTask, nextFreeWorker);
			std::cout << "Master: Sent job to " << nextFreeWorker << std::endl;
			availableWorkers.pop();
			remainingTasks.pop();
		}
	}
	std::cout << "Master committed suicide! Ded!" << std::endl;
}

void Master::initTasks() {
	// Add required tasks to list
	
	// Files to break into words
	for (const auto& entry : std::filesystem::directory_iterator(inputDirectory)) {
		std::string task;
		task += PACKET_MAP_TO_WORDS;
		task += entry.path().filename().u8string();
		task += MESSAGE_SEPARATOR;
		task += inputDirectory;
		task += MESSAGE_SEPARATOR;
		task += temporaryDirectory;
		remainingTasks.push(task);
	}

	// Wait for all of them to finish
	std::string barrier = "";
	barrier += PACKET_BARRIER;
	remainingTasks.push(barrier);

	// Letters to reduce to
	for (char letter = 'a'; letter <= 'z'; ++letter) {
		std::string task;
		task += PACKET_REDUCE_TO_LETTER;
		task += letter;
		task += MESSAGE_SEPARATOR;
		task += temporaryDirectory;
		task += MESSAGE_SEPARATOR;
		task += outputDirectory;
		remainingTasks.push(task);
	}

	// List all tasks
	// Or not, 'cause you're not supposed to see what's in the queue
	//std::cout << "Loaded tasks:" << std::endl;
	//for (const auto& task : remainingTasks) {
	//	std::cout << task << std::endl;
	//}
}

void Master::sendJob(std::string job, int worker) {
	// Do not forget null termination
	MPI_Send(job.c_str(), job.size() + 1, MPI_CHAR, worker, 0, MPI_COMM_WORLD);
}
