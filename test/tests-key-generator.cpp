//
// Created by leandro on 19/07/17.
//

#include <iostream>
#include "../lib/catch.hpp"
#include "../src/key.h"
#include "../lib/key_generator.hpp"
#include "../lib/easylogging++.h"


TEST_CASE( "KeyGen", "[key-generator]" ) {
    key_generator kg(2,10);
    LOG(INFO) << kg;
}
