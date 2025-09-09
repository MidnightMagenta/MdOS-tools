#ifndef MDCF_BACK_H
#define MDCF_BACK_H

#include <fstream>
#include <types.hpp>
#include <vector>

void write_cfg(std::ofstream &f, const std::vector<Entry> &cfgEntries);

#endif