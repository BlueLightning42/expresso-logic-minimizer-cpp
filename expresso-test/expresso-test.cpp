/* Test file to make sure the expresso bridge is working.
 * Linked to the .lib file
*/
#include <iostream>
#include "bridge.h"

int main(){
    std::cout << "First test: " << std::endl;
    auto res = minimize_from_data({ 1,3 }, 3);
    for (const auto s : res) {
        std::cout << s << "\n";
    }
    std::cout << "Second test: " << std::endl;
    res = minimize_from_data({ 1,2,7,8,9 }, 4);
    for (const auto s : res) {
        std::cout << s << "\n";
    }
    std::cout << "Final test.1: " << std::endl;
    res = minimize_from_data({
        ".i 4",
        ".o 1",
        ".type f",
        "0000 0",
        "0001 1",
        "0010 0",
        "0011 1",
        "0100 0",
        "0101 1",
        "0110 0",
        "0111 1",
        "1000 0",
        "1001 1",
        "1010 0",
        "1011 1",
        "1100 0",
        "1101 0",
        "1110 0",
        "1111 1",
        ".e"
    });
    for ( const auto s : res ) {
        std::cout << s << "\n";
    }
    std::cout << "Final test.2: " << std::endl;
    res = minimize_from_data({
        ".i 4",
        ".o 1",
        ".type f",
        "0001 1",
        "0011 1",
        "0101 1",
        "0111 1",
        "1001 1",
        "1011 1",
        "1111 1",
        ".e"
    }); // exact same as above but only the ones. Therefore by default expresso doesn't need to be passed the zeros?
    for ( const auto s : res ) {
        std::cout << s << "\n";
    }
}

