/*
 * -----------------------------------------------------------------------------
 * File: utils.hpp
 * -----------------------------------------------------------------------------
 * Author: Mincho Paskaleff
 * -----------------------------------------------------------------------------
 */

#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include "../src/stack.cpp"

/*
 * Utilities for common use
 */
namespace UTILS {

    const int START_STATE = 0;

    const int ALPHABET_SIZE = 37;

    const int MAX_SIZE = 100;

    const char EPS = 'E';

    const char LEFT_BRACKET = '(';

    const char RIGHT_BRACKET = ')';

    const char KLEENE = '*';

    const char PLUS = '+';

    const char CONCAT = '.';

    const char UNION = '|';


    const char ALPHABET[] = { EPS, '0', '1', '2', '3', '4', '5', '6', '7', '8',
                           '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                           'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
                           't', 'u', 'v', 'w', 'x', 'y', 'z'};

    template <typename T>
    bool isInSet(const T* set, int count, T value) {
        for(int i = 0; i < count; ++i)
            if(set[i] == value)
                return true;
        return false;
    }

}

#endif /* UTILS_H */
