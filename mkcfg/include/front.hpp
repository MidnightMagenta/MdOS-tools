#ifndef MDCF_FRONT_HPP
#define MDCF_FRONT_HPP

#include <types.hpp>
#include <fstream>
#include <string>
#include <vector>

std::string trim(const std::string &source);
Entry make_entry(const std::string &typeStr, const std::string &dataStr);
std::vector<Entry> read_config(std::ifstream &in);

#endif
