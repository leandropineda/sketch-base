//
// Created by leandro on 15/06/17.
//

#include <iostream>
#include "../lib/catch.hpp"
#include "../src/key.h"

TEST_CASE( "Compare different pairs of keys" ) {
    key *k1, *k2;
    k1 = new key(9,9,9,9); k2 = new key(9,9,9,9);
    REQUIRE( (*k1<*k2) == false );
    delete k1, k2;
    k1 = new key(1,9,9,9); k2 = new key(9,9,9,9);
    REQUIRE( (*k1<*k2) == true );
    delete k1, k2;
    k1 = new key(9,1,9,9); k2 = new key(9,9,9,9);
    REQUIRE( (*k1<*k2) == true );
    delete k1, k2;
    k1 = new key(9,9,1,9); k2 = new key(9,9,9,9);
    REQUIRE( (*k1<*k2) == true );
    delete k1, k2;
    k1 = new key(9,9,9,1); k2 = new key(9,9,9,9);
    REQUIRE( (*k1<*k2) == true );
    delete k1, k2;

    k1 = new key(9,9,9,9); k2 = new key(1,9,9,9);
    REQUIRE( (*k1<*k2) == false );
    delete k1, k2;
    k1 = new key(9,9,9,9); k2 = new key(9,1,9,9);
    REQUIRE( (*k1<*k2) == false );
    delete k1, k2;
    k1 = new key(9,9,9,9); k2 = new key(9,9,1,9);
    REQUIRE( (*k1<*k2) == false );
    delete k1, k2;
    k1 = new key(9,9,9,9); k2 = new key(9,9,9,1);
    REQUIRE( (*k1<*k2) == false );
    delete k1, k2;

    k1 = new key(1,1,9,9); k2 = new key(1,1,9,1);
    REQUIRE( (*k1<*k2) == false );
    delete k1, k2;
    k1 = new key(1,1,9,1); k2 = new key(1,1,9,9);
    REQUIRE( (*k1<*k2) == true );
    delete k1, k2;

    k1 = new key(14,10,10,10); k2 = new key(10,10,14,10);
    REQUIRE( (*k1<*k2) == false );
    REQUIRE( (*k1<=*k2) == false );
    REQUIRE( (*k1>*k2) == true );
    REQUIRE( (*k1>=*k2) == true );
    delete k1, k2;

}
