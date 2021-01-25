#pragma once
#include <vector>
#include <queue>
#include <string>

class Master {
public:

	Master(int argc, char* argv[], int workerCount);
	~Master();

	void run();
	void initTasks();
	void sendJob(std::string job, int worker);
private:

	int workerCount;
	std::queue<int> availableWorkers;
	std::queue<std::string> remainingTasks;
	std::string inputDirectory;
	std::string outputDirectory;
};
