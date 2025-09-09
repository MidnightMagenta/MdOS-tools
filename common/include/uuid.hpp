#ifndef MDCF_UUID_H
#define MDCF_UUID_H

#include <cstdint>
#include <string>

struct GUID {
	uint32_t d1;
	uint16_t d2;
	uint16_t d3;
	uint8_t d4[8];
} __attribute__((packed));

bool get_uuid_from_string(const std::string &str, GUID *uuid);

#endif