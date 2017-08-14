//
// Created by leandro on 15/06/17.
//

#ifndef SKETCH_EVENTCOUNTER_H
#define SKETCH_EVENTCOUNTER_H

#include <ostream>
#include <iomanip>
#include <map>
#include <set>
#include "../lib/easylogging++.h"
#include "../lib/utils.hpp"
#include "../lib/typedefs.h"


class bucket {
private:
    uint32_t V, err;
    event_counter_t *event_counter;
    uint32_t max_length;
public:
    bucket(uint32_t max_length);

    void addElement(key &e);
    uint32_t getEventCounterMinValue();
    uint32_t getCounterValue() const;
    estimation_t estimateElementFrequency(key &e);
    key_set_t getKeySet() const;

    friend std::ostream &operator<<(std::ostream &os, const bucket &__eventcounter);
};


#endif //SKETCH_EVENTCOUNTER_H
