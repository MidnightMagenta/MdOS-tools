#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <types.hpp>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Invalid argument\n";
		return EXIT_FAILURE;
	}

	std::string inPath = std::string(argv[1]);
	std::ifstream f(inPath, std::ios::binary | std::ios::ate);
	if (!f.is_open()) {
		std::cerr << "Could not open file " << inPath << "\n";
		return EXIT_FAILURE;
	}

	size_t fSize = f.tellg();
	f.seekg(0);

	if (fSize < sizeof(ConfigHeader)) {
		std::cerr << "Invalid config header\n";
		f.close();
		return EXIT_FAILURE;
	}

	ConfigHeader h;
	memset(&h, 0x00, sizeof(ConfigHeader));
	f.read((char *) &h, sizeof(ConfigHeader));

	if (std::memcmp(&h.magic, "MBCF", 4) != 0) {
		std::cerr << "Failed to verify header magic\n";
		f.close();
		return EXIT_FAILURE;
	}

	std::cout << std::left << std::setw(20) << "Version:" << +h.version << "\n";
	std::cout << std::left << std::setw(20) << "crc32:" << std::hex << "0x" << h.crc32 << std::dec << "\n";
	std::cout << std::left << std::setw(20) << "Data offset:" << std::hex << "0x" << h.dataOffset << std::dec << "\n";

	size_t dataSize = fSize - h.dataOffset;
	char *data = new char[dataSize];
    f.seekg(h.dataOffset);
	f.read(data, dataSize);
	crc32_t crc = crc32(data, dataSize);

	if (crc != h.crc32) {
		std::cout << "Could not verify CRC32\n";
		delete[] data;
		f.close();
		return EXIT_FAILURE;
	}

	std::cout << "CRC32 valid\n";
	delete[] data;
	f.close();
	return EXIT_SUCCESS;
}