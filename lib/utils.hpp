//
// Created by leandro on 24/07/17.
//

#ifndef SKETCH_UTILS_HPP
#define SKETCH_UTILS_HPP

#include "typedefs.h"

inline key getRandomKey(uint32_t max = UINT32_MAX) {
    return rand() % max;
}

#endif //SKETCH_UTILS_HPP
