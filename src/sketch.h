//
// Created by leandro on 17/06/17.
//

#ifndef SKETCH_SKETCH_H
#define SKETCH_SKETCH_H

#include <vector>
#include <set>
#include <stdlib.h>
#include <assert.h>
#include <ostream>
#include <iostream>
#include <iomanip>
#include <algorithm>    // std::max
#include "bucket.h"
#include "../lib/easylogging++.h"

class sketch {
    uint32_t w, d, p;
    uint32_t max_dict_length;
    std::vector<hash_func_t> hash_func;
    bucket_list_t *matrix;

private:
    void __allocSketch();
    void __buildHashFunc();
    uint32_t __hashKey(uint32_t a, uint32_t b, key &k);
    uint32_t __index(uint32_t i, uint32_t j) const;
    bool __isPrime(uint32_t n) const; //TODO: implement and add this constrain to class constructor
    bool __doesTheKeyExceedsTheGivenThresholdOnAllBuckets(key &k, uint32_t threshold);
    bool __doesTheDifferenceBetweenFrequencyEstimationForTheKeyExceedsTheGivenThresholdOnAllBuckets(key &k, sketch sk, uint32_t threshold);
public:
    sketch(uint32_t d, uint32_t w, uint32_t p, uint32_t max_dict_length = 8);
    ~sketch();

    void clear();
    void addElement(key &k);
    uint32_t countElements();
    key_set_t getHeavyHitters(uint32_t threshold);
    key_set_t getHeavyChangers(sketch &sk, uint32_t threshold);

    friend std::ostream &operator<<(std::ostream &os, const sketch &sk);

};


#endif //SKETCH_SKETCH_H
