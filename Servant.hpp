#pragma once

#include <string>

class Servant {
public:

	Servant(int workerId);
	~Servant();

	void run();
	void notifyMaster();
	std::string getNextTask();
private:

	int workerId;
};