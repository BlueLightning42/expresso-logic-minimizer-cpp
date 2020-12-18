#ifndef __ESPRESSO_BRIDGE_H
#define __ESPRESSO_BRIDGE_H

#include <vector>
#include <string>

// extern void init_runtime(void);

std::vector<std::string> minimize_from_data(std::vector<size_t> ones, size_t truth_table_width);
std::vector<std::string> minimize_from_data(std::vector<std::string> data);

#endif
