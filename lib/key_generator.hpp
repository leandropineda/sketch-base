//
// Created by leandro on 18/07/17.
//

#ifndef SKETCH_KEY_GENERATOR_HPP
#define SKETCH_KEY_GENERATOR_HPP

#include <sys/types.h>
#include <set>
#include "../src/key.h"
#include "utils.hpp"
#include "typedefs.h"


class key_generator {
private:
    uint n_different_keys;
    uint n_keys;
    key_set_t keySet;

    void __generateKeySet() {
        while (keySet.size() < n_different_keys) {
            keySet.insert(getRandomKey());
        }
    }


public:
    key_generator(uint _n_different_keys, uint _n_keys): n_different_keys(_n_different_keys), n_keys(_n_keys) {
        srand (time(NULL));
        __generateKeySet();
    }

    friend std::ostream &operator<<(std::ostream &os, const key_generator &generator) {
        for (key_set_t::iterator it = generator.keySet.begin(); it != generator.keySet.end(); it++) {
            os << *it << " ";
        }

        return os;
    }
};

#endif //SKETCH_KEY_GENERATOR_HPP
