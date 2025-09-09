#include "../include/front.hpp"
#include "../include/uuid.hpp"
#include <codecvt>
#include <cstdint>
#include <cstring>
#include <locale>
#include <sstream>

std::string trim(const std::string &source) {
	std::string s(source);

	auto start = s.find_first_not_of(" \n\r\t");
	if (start == std::string::npos) return "";

	auto end = s.find_last_not_of(" \n\r\t");

	return s.substr(start, end - start + 1);
}

Entry make_entry(const std::string &typeStr, const std::string &dataStr) {
	Entry e;

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
		throw std::runtime_error("Unknown type " + typeStr);
	}

	return e;
}

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

		Entry e = make_entry(typeStr, dataStr);
		e.key = key;
		entries.push_back(std::move(e));
	}

	return entries;
}
