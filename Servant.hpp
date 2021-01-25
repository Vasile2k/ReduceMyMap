#pragma once

#include <string>
#include <vector>
#include "Common.hpp"

class Servant {
public:

	Servant(int workerId);
	~Servant();

	void run();
	void notifyMaster();
	std::string getNextTask();

	void mapFileToWords(std::string filename, std::string inputDirectory, std::string outputDirectory);
	void reduceLetter(char letter, std::string inputDirectory, std::string outputDirectory);
private:

	int workerId;
};

/*
 * Helper function to split strings
 */
std::vector<std::string> split(const std::string& str, const std::string& delim = &MESSAGE_SEPARATOR);