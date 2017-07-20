//
// Created by leandro on 25/06/17.
//

#include <iostream>
#include "../lib/catch.hpp"
#include "../src/key.h"
#include "../src/bucket.h"
#include "../lib/utils.hpp"

uint n = 100;

TEST_CASE( "Add the same element multiple times" , "[bucket]") {
    bucket b(2);
    key k = getRandomKey();
    for (int i=0; i<n; i++) {
        b.addElement(k);
    }

    LOG(INFO) << b;

    REQUIRE( b.getCounterValue() == n );
}

TEST_CASE( "Add two different elements multiple times" , "[bucket]") {
    bucket b(2);
    key k1 = getRandomKey();
    key k2 = getRandomKey();
    for (int i = 0; i < n; i++) {
        b.addElement(k1);
        b.addElement(k2);
    }

    LOG(INFO) << b;

    REQUIRE( b.getCounterValue() == 2*n );
}

TEST_CASE( "Add different elements multiple times" , "[bucket]") {
    bucket b(5);
    key k1 = getRandomKey();
    key k2 = getRandomKey();
    key k3 = getRandomKey();
    key k4 = getRandomKey();
    for (int i=0; i<n; i++) { b.addElement(k1); }
    for (int i=0; i<n; i++) { b.addElement(k2); }
    for (int i=0; i<n; i++) { b.addElement(k3); }
    for (int i=0; i<n; i++) { b.addElement(k4); }

    LOG(INFO) << b;

    REQUIRE( b.getCounterValue() == 4*n );
}

TEST_CASE( "Add different elements multiple times on a small bucket", "[bucket]") {
    bucket b(3);
    key k1 = getRandomKey();
    key k2 = getRandomKey();
    key k3 = getRandomKey();
    key k4 = getRandomKey();
    for (int i=0; i<n; i++) { b.addElement(k1); }
    for (int i=0; i<n; i++) { b.addElement(k2); }
    for (int i=0; i<n; i++) { b.addElement(k3); }
    for (int i=0; i<n; i++) { b.addElement(k4); }
    for (int i=0; i<n; i++) { b.addElement(k2); }
    for (int i=0; i<n; i++) { b.addElement(k1); }

    LOG(INFO) << b;

    REQUIRE( b.getCounterValue() == 6*n );

}

TEST_CASE( "Estimate", "[bucket]") {
    bucket b(4);
    key k1 = getRandomKey();
    key k2 = getRandomKey();
    key k3 = getRandomKey();
    key k4 = getRandomKey();
    key k5 = getRandomKey();
    for (int i=0; i<n; i++) { b.addElement(k1); }
    for (int i=0; i<n; i++) { b.addElement(k2); }
    for (int i=0; i<n; i++) { b.addElement(k3); }
    for (int i=0; i<n; i++) { b.addElement(k4); }

    estimation_t f = b.estimateElementFrequency(k1);

    REQUIRE( (n >= f.first && n <= f.second) );

    for (int i=0; i<n; i++) { b.addElement(k5); }

    LOG(INFO) << b.estimateElementFrequency(k1).first << " " << b.estimateElementFrequency(k1).second;
}

TEST_CASE( "Estimate1", "[bucket]") {
    bucket b(10);

    for (int i=0; i<n; i++) {
        key k = getRandomKey();
        b.addElement(k);
    }

    key k = getRandomKey();

    for (int i=0; i<30; i++) {
        b.addElement(k);
    }

    LOG(INFO) << b.estimateElementFrequency(k).first << " " << b.estimateElementFrequency(k).second;
}