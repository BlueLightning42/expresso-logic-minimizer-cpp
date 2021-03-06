#include <stdlib.h>
#include <vector>
#include <string>
#include "bridge.h"
#if defined(_DEBUG)
#include <stdexcept>
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

std::vector<std::string> minimize_from_ones(std::vector<size_t> ones, size_t truth_table_width) {
#if defined(_DEBUG)
    // using runtime errors in _debug instead of assert to be more explicit and because idk if is_sorted will dissapear in assert.
    if ( !( std::is_sorted(ones.begin(), ones.end()) ) ) throw std::runtime_error("ones vector needs to be sorted.");
    if ( truth_table_width < 1 )                         throw std::runtime_error("truth table width cannot be less than 1");
    if ( ones.empty() )                                 throw std::runtime_error("Ones vector should not be empty");
#endif
    std::vector<std::string> PLA;
    PLA.reserve(ones.size() + 4);
    auto one = ones.begin();
    PLA.emplace_back(".i " + std::to_string(truth_table_width));
    PLA.emplace_back(".o 1"); // only supporting one output cause it fits my use case.
    PLA.emplace_back(".type f"); // I only want to supply the ones.
    std::string tmp(truth_table_width + 2, ' ');
    tmp[truth_table_width + 1] = '1';
    for ( const auto& one : ones ) {
        for ( size_t j = 0; j < truth_table_width; j++ ) { // doing it manually cause I don't think theres a standard library way to convert to variable length bitset string.
            tmp[j] = ( ( one >> j ) & 0x1 ) ? '1' : '0';
        }
        PLA.emplace_back(tmp);
    }
    PLA.emplace_back(".e");
    return minimize_from_data(PLA);
}
std::vector<std::string> minimize_from_data(std::vector<std::string> data) {
    size_t length = data.size();
#if defined(_DEBUG)
    if ( data.empty() ) throw std::runtime_error("data vector should not be empty");
#endif
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
    result = run_espresso_from_data(truthTable, (unsigned int)length);
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
