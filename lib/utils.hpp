//
// Created by leandro on 25/06/17.
//

#ifndef SKETCH_UTILS_HPP
#define SKETCH_UTILS_HPP

#include "../src/key.h"

inline key getRandomKey(uint range_ip = 65536, uint range_port = 16384) {
    return key(rand() % range_ip,
               rand() % range_port,
               rand() % range_ip,
               rand() % range_port);
}

#endif //SKETCH_UTILS_HPP
