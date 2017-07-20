//
// Created by leandro on 15/06/17.
//

#include "bucket.h"

bucket::bucket(uint max_length) {
    this->V = 0;
    this->err = 0;
    this->max_length = max_length;
    this->event_counter = new event_counter_t();
}

// this function implements FREQUENT counter-based algorithm
void bucket::addElement(key &e) {
    uint value = 1;
    this->V += value;
    //LOG(DEBUG) << "Adding e " << e;
    // if the e exists on the map
    if (event_counter->find(e) != event_counter->end()) {
        event_counter->at(e) = event_counter->at(e) + value;
    } else {
        if (event_counter->size() < max_length) {
            // event_counter is not full and key is not in event_counter
            event_counter->insert(event_pair_t(e, value));
        } else {
            uint d_err;
            ((value < this->getEventCounterMinValue()) ? d_err = value : d_err = this->getEventCounterMinValue());
            this->err += d_err;
            //LOG(DEBUG) << "Decreasing all elements on event_counter";
            for (event_counter_t::iterator it = event_counter->begin(); it != event_counter->end(); it++) {
                event_counter->at(it->first) = event_counter->at(it->first) - d_err;
                if (event_counter->at(it->first) <= 0) {
                    event_counter->erase(it);
                }
            }
        }
    }
    //LOG(DEBUG) << "Structure data " << *this;
}

std::ostream &operator<<(std::ostream &os, const bucket &__eventcounter) {
    os << "V:" << std::setw(4) << __eventcounter.getCounterValue()
       << " E:" << std::setw(4) << __eventcounter.err << std::setw(4)
       << " - "<< __eventcounter.getKeySet().size() << " key(s): ";
    event_counter_t::iterator it;
    for (it = __eventcounter.event_counter->begin(); it != __eventcounter.event_counter->end(); it++) {
        os << it->first << ":" << it->second << " ";
    }
    return os;
}

uint bucket::getEventCounterMinValue() {
    uint min = UINT32_MAX;
    for (event_counter_t::iterator it = this->event_counter->begin(); it != this->event_counter->end(); it++) {
        if (it->second < min) min = it->second;
    }
    return min;
}

uint bucket::getCounterValue() const {
    return this->V;
}

estimation_t bucket::estimateElementFrequency(key &e) {
    estimation_t estimation;
    if (this->event_counter->find(e) != this->event_counter->end()) {
        estimation.first = this->event_counter->at(e);
    } else estimation.first = 0;

    estimation.second = estimation.first + this->err;

    return estimation;
}

key_set_t bucket::getKeySet() const {
    key_set_t key_set;
    for (event_counter_t::iterator it = this->event_counter->begin(); it != this->event_counter->end(); it++) {
        key_set.insert(it->first);
    }
    return key_set;
}

