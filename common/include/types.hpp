#ifndef MDCF_TYPES_H
#define MDCF_TYPES_H

#include <crc32.hpp>
#include <string>
#include <vector>
#include <cstdint>

enum class ValueType { INT8, INT16, INT32, INT64, UINT8, UINT16, UINT32, UINT64, BYTES, UTF8, UTF16, GUID };

struct Entry {
	std::string key;
	ValueType type;
	std::vector<uint8_t> data;
};

#define MDBOOTCFG_MAGIC {'M', 'B', 'C', 'F'}

enum MDBOOTCFG_VERSION : uint8_t { MDBOOTCFG_VERSION_INVALID = 0, MDBOOTCFG_VERSION1 = 1 };

struct ConfigHeader {
	uint8_t magic[4] = MDBOOTCFG_MAGIC;
	crc32_t crc32;
	uint8_t version = MDBOOTCFG_VERSION1;
	uint8_t pad[7] = {};
	uint64_t dataOffset;
} __attribute__((packed));

static_assert(sizeof(ConfigHeader) == 24);

#endif