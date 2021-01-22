#pragma once

class Servant {
public:

	Servant(int workerId);
	~Servant();

	void run();
private:

	int workerId;
};