#include "Servant.hpp"
#include <iostream>

Servant::Servant(int workerId) : workerId(workerId) {
	std::cout << "Worker " << workerId << " created" << std::endl;
}

Servant::~Servant() {

}

void Servant::run() {

}
