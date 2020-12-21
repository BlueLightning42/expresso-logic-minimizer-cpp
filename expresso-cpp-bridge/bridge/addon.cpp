#include <stdlib.h>
#include <vector>
#include <string>
#include <cmath>
#include "bridge.h"
#if defined(_DEBUG)
#include <cassert>
#include <algorithm>
#endif

// edited from https://github.com/scottinet/espresso-logic-minimizer/blob/master/bridge/addon.cc

extern "C" char** run_espresso_from_data(char** data, unsigned int length);
extern "C" char** run_espresso_from_path(char* path);

// if input is array of minterms. Then minterms will be converted to pla in memory and solved.
/* ie passing
 * 2 and
 * [1,3] with the vector being sorted.
 */
 // will be converted to 
 /*
  * .i 2
  * .o 1
  * 00 0
  * 10 1
  * 01 0
  * 11 1
  * .e
  */
  // and then passed to the other overload.

std::vector<std::string> minimize_from_data(std::vector<size_t> ones, size_t truth_table_width) {
#if defined(_DEBUG)
    // in debug block even though assert should be disabled in release because idk if is_sorted will run in release
    assert(( std::is_sorted(ones.begin(), ones.end()) ));
    assert(( "Truth Table Width is less than 1", ( truth_table_width >= 1 ) ));
    assert(( "Ones vector should not be empty", ( !ones.empty() ) ));
#endif
    const auto truth_table_size = (size_t)std::pow((size_t)2, truth_table_width); // 2^width
    std::vector<std::string> PLA;
    PLA.reserve(truth_table_size + 3);
    auto one = ones.begin();
    PLA.emplace_back(".i " + std::to_string(truth_table_width));
    PLA.emplace_back(".o 1"); // only supporting one output cause it fits my use case.
    for ( size_t i = 0; i < truth_table_size; i++ ) {
        std::string tmp(truth_table_width + 2, ' ');
        for ( size_t j = 0; j < truth_table_width; j++ ) { // doing it manually cause I don't think theres a standard library way to convert to variable length bitset string.
            tmp[j] = ( ( i >> j ) & 0x1 ) ? '1' : '0';
        }
        if ( one != ones.end() && *one == i ) {
            tmp[truth_table_width + 1] = '1';
            one++;
        } else {
            tmp[truth_table_width + 1] = '0';
        }
        PLA.emplace_back(tmp);
    }
    PLA.emplace_back(".e");
    return minimize_from_data(PLA);
}
std::vector<std::string> minimize_from_data(std::vector<std::string> data) {
    unsigned int length = data.size();
    assert(( "data vector should not be empty", ( length >= 1 ) ));
    char** truthTable, ** result;

    truthTable = new char* [length];
    for ( size_t i = 0; i < length; ++i ) {
        truthTable[i] = new char[data[i].size() + 1];
#if defined(_MSC_VER)
        strcpy_s(truthTable[i], data[i].size() + 1, data[i].c_str());
#else
        strcpy(truthTable[i], data[i].c_str());
#endif // _MSC_VER
    }
    result = run_espresso_from_data(truthTable, length);
    std::vector<std::string> return_val;
    if ( result != NULL ) {
#pragma warning(suppress: 6001)
        for ( unsigned int i = 0; result[i] != NULL; ++i ) {
            return_val.emplace_back(result[i]);
            // since the result comes from C code, the memory was
            // allocated using malloc and must be freed with free
            free(result[i]);
        }
        free(result);
    }
    // memory clean up
    /*
    for(unsigned int i = 0; i < length; delete truthTable[i++]);
    delete truthTable;
    */
    delete[] truthTable; // visual studio likes this more?
    return return_val;
}
