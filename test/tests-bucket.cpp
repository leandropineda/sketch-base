//
// Created by leandro on 25/06/17.
//


#include <iostream>
#include "../lib/catch.hpp"
#include "../src/bucket.h"
#include <random>


SCENARIO( "add the same element multiple times to a bucket big enough" , "[bucket]") {
    uint MAX_N = 100000;
    uint n_elem = rand() % MAX_N;
    uint max_keys = rand() % (uint) 10 + 1;
    GIVEN ("A bucket of length " + std::to_string(max_keys)) {
        bucket b(max_keys);
        WHEN ("a random key is created") {
            key k = getRandomKey();
            AND_WHEN (std::to_string(n_elem) + " elements are added to the bucket") {
                for (int i = 0; i < n_elem; i++) b.addElement(k);
                THEN ("the size of the bucket is " + std::to_string(n_elem)) {
                    REQUIRE(b.getCounterValue() == n_elem);
                    AND_THEN("the amount of different elements is 1") {
                        REQUIRE(b.getKeySet().size() == 1);
                    }
                }
            }
        }
    }
}

SCENARIO( "add multiple elements multiple times to a bucket just big enough" , "[bucket]") {
    uint MAX_N = 100000;
    uint n_elem = rand() % MAX_N;
    uint max_keys = rand() % (uint) 10 + 2;
    GIVEN ("A bucket of length " + std::to_string(max_keys)) {
        bucket b(max_keys);
        WHEN ("a set of " + std::to_string(max_keys) + " random keys is created") {
            std::set<key> keys;
            while (keys.size() < max_keys) {
                key k = getRandomKey();
                keys.insert(k);
            }
            AND_WHEN(std::to_string(n_elem) + " of each element is added to the bucket") {
                for (std::set<key>::iterator it = keys.begin(); it != keys.end(); it++) {
                    key k = *it;
                    for (int i = 0; i < n_elem; i++) b.addElement(k);
                }
                THEN("the size of the bucket is " + std::to_string(n_elem * max_keys)) {
                    REQUIRE(b.getCounterValue() == max_keys * n_elem);
                    AND_THEN("the amount of different keys is " + std::to_string(n_elem)) {
                        REQUIRE(b.getKeySet().size() == max_keys);
                    }

                }

            }
        }
    }
}

SCENARIO( "add multiple elements multiple times on a small bucket", "[bucket]") {
    uint MAX_N = 100000;
    uint max_keys = rand() % (uint) 10 + 20;
    uint n_elem = (rand() % MAX_N);
    uint extra_keys = rand() % (uint) 10 + 1;
    GIVEN ("A bucket of length " + std::to_string(max_keys)) {
        bucket b(max_keys);
        WHEN ("a set of " + std::to_string(max_keys) + " random keys is created") {
            std::set<key> keys;
            while (keys.size() < max_keys + extra_keys) keys.insert(getRandomKey());
            AND_WHEN(std::to_string(n_elem) + " of each element is added to the bucket") {
                for (std::set<key>::iterator it = keys.begin(); it != keys.end(); it++) {
                    for (uint i = 0; i < n_elem; i++) {
                        key _k = *it;
                        b.addElement(_k);
                    }
                }
                THEN("the size of the bucket is " + std::to_string((max_keys + extra_keys) * n_elem)) {
                    REQUIRE(b.getCounterValue() == (max_keys + extra_keys) * n_elem);
                    AND_THEN("the amount of different keys is " + std::to_string(extra_keys - 1)) {
                        REQUIRE(b.getKeySet().size() == extra_keys - 1);
                    }
                }
            }
        }
    }
}

SCENARIO( "estimate the frequency of an element", "[bucket]") {
    uint MAX_N = 100000;
    for (uint j = 0; j < 10; j++) {
        uint max_keys = rand() % (uint) 20 + 5;
        uint n_elem = (rand() % MAX_N) + MAX_N;
        uint extra_keys = 10;
        GIVEN("a bucket of length " + std::to_string(max_keys)) {
            bucket b(max_keys);
            WHEN("keys between 0 and " + std::to_string(max_keys + extra_keys) +
                 " are generated with binomial distribution (stddev 0.9)") {
                std::default_random_engine generator;
                std::binomial_distribution<key> distribution(max_keys + extra_keys, 0.5);
                std::map<key, uint> p;
                AND_WHEN(std::to_string(n_elem) + " are inserted into the bucket") {
                    for (int i = 0; i < n_elem; ++i) {
                        key k = distribution(generator);
                        b.addElement(k);
                        ++p[k];
                    }
                    key mean_key = uint((max_keys + extra_keys) / 2);
                    THEN("the frequency for the mean key " + std::to_string(mean_key) + " is " +
                         std::to_string(p[mean_key])) {
                        estimation_t f = b.estimateElementFrequency(mean_key);

                        THEN("the real frequency for the mean key is estimated correctly") {
                            REQUIRE((f.first <= p[mean_key] and f.second >= p[mean_key]));
                            REQUIRE(f.second == p[mean_key]);

                        }
                    }
                }
            }
        }
    }
}
