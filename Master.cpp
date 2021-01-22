#include "Master.hpp"
#include <iostream>
#include <string>

#define INPUT_PATH_ARGUMENT "-i"
#define OUTPUT_PATH_ARGUMENT "-o"

Master::Master(int argc, char* argv[]) : availableWorkers() {
	
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

	if (!(inputPathSet && outputPathSet)) {
		std::cout << "Master: Can't continue, path are not both set!" << std::endl;
	}

	std::cout << "Master created" << std::endl;
}

Master::~Master() {

}

void Master::run() {

}
