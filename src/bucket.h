//
// Created by leandro on 15/06/17.
//

#ifndef SKETCH_EVENTCOUNTER_H
#define SKETCH_EVENTCOUNTER_H

#include <ostream>
#include <iomanip>
#include <map>
#include <set>
#include "key.h"
#include "../lib/easylogging++.h"
#include "../lib/typedefs.h"

class bucket {
private:
    uint V, err;
    event_counter_t *event_counter;
    uint max_length;
public:
    bucket(uint max_length);

    void addElement(key &e);
    uint getEventCounterMinValue();
    uint getCounterValue() const;
    estimation_t estimateElementFrequency(key &e);
    key_set_t getKeySet() const;

    friend std::ostream &operator<<(std::ostream &os, const bucket &__eventcounter);
};


#endif //SKETCH_EVENTCOUNTER_H
