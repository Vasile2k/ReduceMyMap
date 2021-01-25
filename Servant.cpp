#include <nlohmann/json.hpp>
#include <unordered_map>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <mpi.h>
#include "Servant.hpp"
#include "Common.hpp"

Servant::Servant(int workerId) : workerId(workerId) {
	std::cout << "Worker " << workerId << " created" << std::endl;
}

Servant::~Servant() {

}

void Servant::run() {
	bool working = true;
	while (working) {
		// Tell the boss I'm free
		notifyMaster();

		std::string nextJob = getNextTask();
		char jobType = nextJob[0];

		switch (jobType) {
			case PACKET_SUICIDE: {
				working = false;
				break;
			}
			case PACKET_MAP_TO_WORDS: {
				const char* messageData = nextJob.c_str() + 1;
				auto splitt = split(messageData);
				assert(splitt.size() == 3, "Map: Message somehow got fucked up!");

				auto filename = splitt[0];
				auto inputDirectory = splitt[1];
				auto outputDirectory = splitt[2];

				std::cout << "Worker " << workerId << " maps file " << filename << " from " << inputDirectory << " to " << outputDirectory << std::endl;
				mapFileToWords(filename, inputDirectory, outputDirectory);
				break;
			}
			case PACKET_REDUCE_TO_LETTER: {
				const char* messageData = nextJob.c_str() + 1;
				auto splitt = split(messageData);
				assert(splitt.size() == 3, "Reduce: Message somehow got fucked up!");

				auto letter = splitt[0][0];
				auto inputDirectory = splitt[1];
				auto outputDirectory = splitt[2];

				std::cout << "Worker " << workerId << " reduces file with letter " << letter << " from " << inputDirectory << " to " << outputDirectory << std::endl;
				reduceLetter(letter, inputDirectory, outputDirectory);
				break;
			}
		}
	}
	std::cout << "Worker " << workerId << " ended life." << std::endl;
}

void Servant::notifyMaster() {
	char packet[] = { PACKET_WORKER_FREE };
	MPI_Send(packet, 1, MPI_CHAR, MASTER_ID, 0, MPI_COMM_WORLD);
}

std::string Servant::getNextTask() {
	char message[256];
	MPI_Status status;
	MPI_Recv(message, 256, MPI_CHAR, MASTER_ID, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	// Fail-safe
	message[255] = 0;
	return std::string(message);
}

void Servant::mapFileToWords(std::string filename, std::string inputDirectory, std::string outputDirectory) {
	std::ifstream inFile(inputDirectory + filename);

	std::unordered_map<std::string, int> freqs;

	std::string line;
	if (inFile.is_open()) {

		while (std::getline(inFile, line)) {
			// Remove carriage return and new-lines from line
			std::replace(line.begin(), line.end(), '\r', ' ');
			std::replace(line.begin(), line.end(), '\n', ' ');

			auto words = split(line, " ");

			for (auto word: words) {
				freqs[word]++;
			}
		}

		inFile.close();

	} else {
		assert(false, "3 cars of police stolen the file from your disk!");
	}

	std::ofstream outFile(outputDirectory + filename + ".json");

	nlohmann::json j;

	for (auto& wordCount : freqs) {
		j[filename][wordCount.first] = wordCount.second;
	}

	if (outFile.is_open()) {
		std::string data;
		try {
			data = j.dump(4);
		} catch (nlohmann::json::exception ex) {
			// LOST A FUCKING HOUR UNTIL I FOUND THIS BULLSHIT.
			// FUCK NON UTF-8 ENCODED INPUT FILES
			// FUCK MY LIFE :|
			std::cout << ex.what() << std::endl;
		}
		outFile << data;
		outFile.close();
	} else {
		assert(false, "3 cars of police won't let me create the file on your disk!");
	}
}

void Servant::reduceLetter(char letter, std::string inputDirectory, std::string outputDirectory) {
	nlohmann::json j;

	for (const auto& entry : std::filesystem::directory_iterator(inputDirectory)) {
		std::string filename = entry.path().filename().u8string();

		std::ifstream inFile(inputDirectory + filename);
		std::string content((std::istreambuf_iterator<char>(inFile)), (std::istreambuf_iterator<char>()));
		inFile.close();

		nlohmann::json mapped = nlohmann::json::parse(content);

		for (auto& [document, counts] : mapped.items()) {
			for (auto& [word, count] : counts.items()) {
				if (word[0] == letter) {
					j[word][document] = count;
				}
			}
		}
	}

	std::ofstream outFile(outputDirectory + letter + ".json");
	if (outFile.is_open()) {
		std::string data = j.dump(4);
		outFile << data;
		outFile.close();
	} else {
		assert(false, "3 cars of police won't let me create the file on your disk!");
	}
}

std::vector<std::string> split(const std::string& str, const std::string& delim) {
	std::vector<std::string> tokens;
	size_t prev = 0;
	size_t pos = 0;

	do {
		pos = str.find(delim, prev);
		if (pos == std::string::npos) {
			pos = str.length();
		}
		std::string token = str.substr(prev, pos-prev);
		if (!token.empty()) {
			tokens.push_back(token);
		}
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());

	return tokens;
}