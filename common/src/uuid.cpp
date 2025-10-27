#include "../include/uuid.hpp"
#include <iostream>
#include <regex>

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