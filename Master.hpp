#pragma once
#include <vector>
#include <string>

class Master {
public:

	Master(int argc, char* argv[], int workerCount);
	~Master();

	void run();
	void initTasks();
private:

	int workerCount;
	std::vector<int> availableWorkers;
	std::vector<std::string> remainingTasks;
	std::string inputDirectory;
	std::string outputDirectory;
};
