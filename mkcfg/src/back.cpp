#include "../include/back.hpp"

void write_cfg(std::ofstream &f, const std::vector<Entry> &cfgEntries) {
	std::vector<uint8_t> buffer;

	for (const Entry &e : cfgEntries) {
		buffer.insert(buffer.end(), reinterpret_cast<const uint8_t *>(e.key.data()),
					  reinterpret_cast<const uint8_t *>(e.key.data() + e.key.size()));

		buffer.push_back('\0');

		uint16_t s = e.data.size();
		const uint8_t *sp = reinterpret_cast<const uint8_t *>(&s);
		buffer.insert(buffer.end(), sp, sp + sizeof(s));

		buffer.insert(buffer.end(), reinterpret_cast<const uint8_t *>(e.data.data()),
					  reinterpret_cast<const uint8_t *>(e.data.data() + e.data.size()));

		buffer.push_back('\0');
		buffer.push_back('\n');
	}

	ConfigHeader header;
	header.version = MDBOOTCFG_VERSION1;
	header.dataOffset = sizeof(ConfigHeader);
	header.crc32 = crc32(buffer.data(), buffer.size());

	f.write((char *) &header, sizeof(ConfigHeader));
	f.write((char *) buffer.data(), buffer.size());
}