#include <algorithm>
#include <cctype>
#include <codecvt>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <locale>
#include <regex>
#include <string>
#include <vector>

enum class ValueType { INT8, INT16, INT32, INT64, UINT8, UINT16, UINT32, UINT64, BYTES, UTF8, UTF16, GUID };

struct Entry {
	std::string key;
	ValueType type;
	std::vector<uint8_t> data;
};

std::string trim(const std::string &source) {
	std::string s(source);

	auto start = s.find_first_not_of(" \n\r\t");
	if (start == std::string::npos) return "";

	auto end = s.find_last_not_of(" \n\r\t");

	return s.substr(start, end - start + 1);
}


struct GUID {
	uint32_t d1;
	uint16_t d2;
	uint16_t d3;
	uint8_t d4[8];
} __attribute__((packed));

#define GUID_REGEX                                                                                                     \
	R"(^([0-9a-fA-F]{8})-([0-9a-fA-F]{4})-([0-9a-fA-F]{4})-([0-9a-fA-F]{2})([0-9a-fA-F]{2})-([0-9a-fA-F]{12})$)"

bool get_uuid_from_string(const std::string &str, GUID *uuid) {
	std::regex guid_regex(GUID_REGEX);
	std::smatch matches;

	if (!std::regex_match(str, matches, guid_regex) || matches.size() < 7) {
		std::cerr << "Failed to match UUID\n";
		return false;
	}

	auto parse_hex = [](const std::string &s) {
		uint64_t val = 0;
		std::stringstream ss(s);
		ss >> std::hex >> val;
		return val;
	};

	uuid->d1 = static_cast<uint32_t>(parse_hex(matches[1].str()));
	uuid->d2 = static_cast<uint16_t>(parse_hex(matches[2].str()));
	uuid->d3 = static_cast<uint16_t>(parse_hex(matches[3].str()));
	uuid->d4[0] = static_cast<uint8_t>(parse_hex(matches[4].str()));
	uuid->d4[1] = static_cast<uint8_t>(parse_hex(matches[5].str()));

	const std::string &lastPart = matches[6].str();
	for (int i = 0; i < 6; ++i) {
		std::string byteStr = lastPart.substr(i * 2, 2);
		uuid->d4[i + 2] = static_cast<uint8_t>(parse_hex(byteStr));
	}

	return true;
}

// template<typename T>
// T parse_integer(const std::string &s) {
// 	static_assert(std::is_integral<T>::value);

// 	if (s.rfind("0b", 0) == 0) { return static_cast<T>(std::stoull(s.substr(2), nullptr, 2)); }

// 	if constexpr (std::is_signed<T>::value) {
// 		long long v = std::stoll(s, nullptr, 0);
// 		if (v < std::numeric_limits<T>::min() || v > std::numeric_limits<T>::max()) {
// 			throw std::out_of_range("Value out of range for target type");
// 		}
// 		return static_cast<T>(v);
// 	} else {
// 		unsigned long long v = std::stoull(s, nullptr, 0);
// 		if (v > std::numeric_limits<T>::max()) { throw std::out_of_range("Value out of range for target type"); }
// 		return static_cast<T>(v);
// 	}

// 	return 0;
// }

std::vector<Entry> read_config(std::ifstream &in) {
	std::vector<Entry> entries;
	std::string line;
	size_t lineno = 0;

	while (std::getline(in, line)) {
		lineno++;
		line = trim(line);
		if (line.empty() || line[0] == '#') { continue; }

		auto eqPos = line.find('=');
		if (eqPos == std::string::npos) { throw std::runtime_error("Missing '=' on line " + std::to_string(lineno)); }

		std::string key = trim(line.substr(0, eqPos));
		std::string rhs = trim(line.substr(eqPos + 1));

		auto lBrace = rhs.find('{');
		auto rBrace = rhs.find('}');
		if (lBrace == std::string::npos || rBrace == std::string::npos || rBrace < lBrace) {
			throw std::runtime_error("Malformed expression on line " + std::to_string(lineno));
		}

		std::string typeStr = trim(rhs.substr(0, lBrace));
		std::string dataStr = rhs.substr(lBrace + 1, rBrace - lBrace - 1);

		Entry e;
		e.key = key;

		if (typeStr == "INT8") {
			e.type = ValueType::INT8;
			int8_t val = int8_t(std::stoi(dataStr, nullptr, 0));
			e.data.resize(sizeof(val));
			std::memcpy(e.data.data(), &val, sizeof(val));
		} else if (typeStr == "INT16") {
			e.type = ValueType::INT16;
			int16_t val = int16_t(std::stoi(dataStr, nullptr, 0));
			e.data.resize(sizeof(val));
			std::memcpy(e.data.data(), &val, sizeof(val));
		} else if (typeStr == "INT32") {
			e.type = ValueType::INT32;
			int32_t val = int32_t(std::stoi(dataStr, nullptr, 0));
			e.data.resize(sizeof(val));
			std::memcpy(e.data.data(), &val, sizeof(val));
		} else if (typeStr == "INT64") {
			e.type = ValueType::INT64;
			int64_t val = int64_t(std::stoll(dataStr, nullptr, 0));
			e.data.resize(sizeof(val));
			std::memcpy(e.data.data(), &val, sizeof(val));
		} else if (typeStr == "UINT8") {
			e.type = ValueType::UINT8;
			uint8_t val = uint8_t(std::stoul(dataStr, nullptr, 0));
			e.data.resize(sizeof(val));
			std::memcpy(e.data.data(), &val, sizeof(val));
		} else if (typeStr == "UINT16") {
			e.type = ValueType::UINT16;
			uint16_t val = uint16_t(std::stoul(dataStr, nullptr, 0));
			e.data.resize(sizeof(val));
			std::memcpy(e.data.data(), &val, sizeof(val));
		} else if (typeStr == "UINT32") {
			e.type = ValueType::UINT32;
			uint32_t val = uint32_t(std::stoul(dataStr, nullptr, 0));
			e.data.resize(sizeof(val));
			std::memcpy(e.data.data(), &val, sizeof(val));
		} else if (typeStr == "UINT64") {
			e.type = ValueType::UINT64;
			uint64_t val = uint64_t(std::stoull(dataStr, nullptr, 0));
			e.data.resize(sizeof(val));
			std::memcpy(e.data.data(), &val, sizeof(val));
		} else if (typeStr == "UTF8") {
			e.type = ValueType::UTF8;
			e.data.assign(dataStr.begin(), dataStr.end());
			e.data.push_back(0);
		} else if (typeStr == "UTF16") {
			e.type = ValueType::UTF16;
			std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
			std::u16string str = convert.from_bytes(dataStr);
			e.data.resize(str.size() * sizeof(char16_t));
			std::memcpy(e.data.data(), str.data(), e.data.size());
			e.data.push_back(0);
			e.data.push_back(0);
		} else if (typeStr == "GUID") {
			e.type = ValueType::GUID;
			GUID guid;
			get_uuid_from_string(dataStr, &guid);
			e.data.resize(sizeof(guid));
			std::memcpy(e.data.data(), &guid, sizeof(guid));
		} else if (typeStr == "BYTES") {
			e.type = ValueType::BYTES;
			std::stringstream data(dataStr);
			std::string byte;
			while (std::getline(data, byte, ',')) {
				byte = trim(byte);
				uint8_t b = uint8_t(std::stoul(byte, nullptr, 0));
				e.data.push_back(b);
			}
		} else {
			throw std::runtime_error("Unknown type on line " + std::to_string(lineno));
		}

		entries.push_back(std::move(e));
	}

	return entries;
}

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

	uint16_t s = 0;
	char terminator[] = {'\0', '\n'};
	char null = '\0';
	for (const Entry &e : cfgEntries) {
		outFile.write(e.key.data(), e.key.size());
		outFile.write(&null, 1);
		s = e.data.size();
		outFile.write((char *) &s, sizeof(s));
		outFile.write((char *) e.data.data(), e.data.size());
		outFile.write(terminator, 2);
	}
	outFile.close();
	return EXIT_SUCCESS;
}