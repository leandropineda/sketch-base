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
    uint w, d, p;
    uint max_dict_length;
    std::vector<hash_func_t> hash_func;
    bucket_list_t *matrix;

private:
    void __allocSketch();
    void __buildHashFunc();
    uint __hashKey(uint a, uint b, key &k);
    uint __index(uint i, uint j) const;
    bool __isPrime(uint n) const; //TODO: implement and add this constrain to class constructor
    bool __doesTheKeyExceedsTheGivenThresholdOnAllBuckets(key &k, uint threshold);
    bool __doesTheDifferenceBetweenFrequencyEstimationForTheKeyExceedsTheGivenThresholdOnAllBuckets(key &k, sketch sk, uint threshold);
public:
    sketch(uint d, uint w, uint p, uint max_dict_length = 8);
    ~sketch();

    void clear();
    void addElement(key &k);
    uint countElements();
    key_set_t getHeavyHitters(uint threshold);
    key_set_t getHeavyChangers(sketch &sk, uint threshold);

    friend std::ostream &operator<<(std::ostream &os, const sketch &sk);

};


#endif //SKETCH_SKETCH_H
