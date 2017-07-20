//
// Created by leandro on 15/06/17.
//

#include "key.h"

std::ostream &operator<<(std::ostream &os, const key &__key) {
    in_addr _src, _dst;
    _src.s_addr=__key.src_ip;
    _dst.s_addr=__key.dst_ip;

    char srcip[100];
    strcpy(srcip, inet_ntoa(_src));
    char dstip[100];
    strcpy(dstip, inet_ntoa(_dst));
    os << "<" << srcip << "," << __key.src_port
       << "," << dstip << "," << __key.dst_port << ">";
    return os;
}

bool operator<(const key &lhs, const key &rhs) {
    if (lhs.src_ip > rhs.src_ip) return false;
        else if (lhs.src_ip < rhs.src_ip) return true;
    if (lhs.src_port > rhs.src_port) return false;
        else if (lhs.src_port < rhs.src_port) return true;
    if (lhs.dst_ip > rhs.dst_ip) return false;
        else if (lhs.dst_ip < rhs.dst_ip) return true;
    if (lhs.dst_port > rhs.dst_port) return false;
        else if (lhs.dst_port < rhs.dst_port) return true;
    return false;
}

bool operator>(const key &lhs, const key &rhs) {
    return rhs < lhs;
}

bool operator<=(const key &lhs, const key &rhs) {
    return !(rhs < lhs);
}

bool operator>=(const key &lhs, const key &rhs) {
    return !(lhs < rhs);
}
