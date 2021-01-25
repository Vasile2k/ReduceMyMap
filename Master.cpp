#include "Master.hpp"
#include <iostream>
#include <string>
#include <filesystem>
#include "Common.hpp"

Master::Master(int argc, char* argv[], int workerCount) : availableWorkers(), workerCount(workerCount) {
	
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

}

void Master::initTasks() {
	// Add required tasks to list
	
	// Files to break into words
	for (const auto& entry : std::filesystem::directory_iterator(inputDirectory)) {
		std::string task;
		task += PACKET_MAP_TO_WORDS;
		task += entry.path().filename().u8string();
		remainingTasks.push_back(task);
	}

	// Wait for all of them to finish
	std::string barrier = "";
	barrier += PACKET_BARRIER;
	remainingTasks.push_back(barrier);

	// Letters to reduce to
	for (char letter = 'a'; letter <= 'z'; ++letter) {
		std::string task;
		task += PACKET_REDUCE_TO_LETTER;
		task += letter;
		remainingTasks.push_back(task);
	}

	// List all tasks
	std::cout << "Loaded tasks:" << std::endl;
	for (const auto& task : remainingTasks) {
		std::cout << task << std::endl;
	}
}
