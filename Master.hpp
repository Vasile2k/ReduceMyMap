#pragma once
#include <vector>
#include <string>

class Master {
public:

	Master(int argc, char* argv[]);
	~Master();

	void run();
private:

	std::vector<int> availableWorkers;
	std::string inputDirectory;
	std::string outputDirectory;
};
