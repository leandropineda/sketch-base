//
// Created by leandro on 15/06/17.
//

#ifndef SKETCH_KEY_H
#define SKETCH_KEY_H

#include <ostream>
#include <bitset>
#include <arpa/inet.h>
#include <cstring>


class key {
public:
    uint src_ip;
    uint dst_ip;
    uint src_port;
    uint dst_port;

    key(uint src_ip, uint src_port, uint dst_ip, uint dst_port):
            src_ip(src_ip), src_port(src_port), dst_ip(dst_ip), dst_port(dst_port) {}

    friend bool operator<(const key &lhs, const key &rhs);
    friend bool operator>(const key &lhs, const key &rhs);
    friend bool operator<=(const key &lhs, const key &rhs);
    friend bool operator>=(const key &lhs, const key &rhs);

    friend std::ostream &operator<<(std::ostream &os, const key &__key);
};


#endif //SKETCH_KEY_H
