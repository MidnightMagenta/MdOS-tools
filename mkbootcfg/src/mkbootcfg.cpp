#include "../include/front.hpp"
#include "../include/types.hpp"
#include "../include/uuid.hpp"
#include "../include/back.hpp"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
	if (argc == 1) {
		std::cerr << "No output file\n";
		return EXIT_FAILURE;
	}

	std::string inputPath;
	std::string outputPath;

	for (int i = 1; i < argc; i++) {
		if (std::strcmp("-i", argv[i]) == 0) {
			if (!inputPath.empty()) {
				std::cerr << "Invalid parameters\n";
				return EXIT_FAILURE;
			}
			inputPath = std::string(argv[++i]);
		} else if (std::strcmp("-o", argv[i]) == 0) {
			if (!outputPath.empty()) {
				std::cerr << "Invalid parameters\n";
				return EXIT_FAILURE;
			}
			outputPath = std::string(argv[++i]);
		} else {
			std::cerr << "Unknown parameter " << argv[i] << "\n";
			return EXIT_FAILURE;
		}
	}

	std::ifstream inFile(inputPath);
	if (!inFile.is_open()) {
		std::cerr << "Could not open input file\n";
		return EXIT_FAILURE;
	}

	std::vector<Entry> cfgEntries;
	try {
		cfgEntries = read_config(inFile);
	} catch (const std::runtime_error &e) {
		std::cerr << e.what() << "\n";
		return EXIT_FAILURE;
	} catch (...) {
		std::cerr << "Unknown error\n";
		return EXIT_FAILURE;
	}

	inFile.close();

	std::ofstream outFile(outputPath, std::ios::binary | std::ios::trunc);
	if (!outFile.is_open()) {
		std::cerr << "Could not open output file\n";
		return EXIT_FAILURE;
	}

	write_cfg(outFile, cfgEntries);

	return EXIT_SUCCESS;
}