//
// Created by leandro on 15/06/17.
//

#include <iostream>
#include "../lib/catch.hpp"
#include "../src/key.h"
#include "../src/sketch.h"
#include "../lib/utils.hpp"
#include "../lib/pcap_parser.hpp"


TEST_CASE( "Add random elements", "[sketch1]" ) {
    sketch sk(5, 10, 1337);
    uint n_elem = 10000;
    srand(0);
    for (uint i = 0; i < n_elem; i++) {
        key k = getRandomKey(16,8);
        sk.addElement(k);
    }
    key k = getRandomKey(16,8);
    for (uint i = 0; i < n_elem/10; i++) {
        sk.addElement(k);
    }

    std::cout<<sk;

    key_set_t key_set = sk.getHeavyHitters(5000);

    for (key_set_t::iterator it = key_set.begin(); it != key_set.end(); it++) {
        std::cout<<*it << " ";
    }


    //REQUIRE( sk.countElements() == n_elem );
}

TEST_CASE( "Heavy Hitters", "[sketch1]" ) {

    pcap_parser pcap_file = pcap_parser("/home/leandro/CLionProjects/sketch/test/pcap-files/smallFlows.pcap");
    pcap_parser_stats pcap_stats;
    sketch sk(5, 100, 9091);

    // reading pcap file
    packet_info p;
    while (pcap_file.has_next()) {
        if (!pcap_file.next_packet(&p)) continue;
        in_addr_t _src = p.ip_src.s_addr;
        in_addr_t _dst = p.ip_dst.s_addr;
        key key(_src, p.srcport, _dst, p.dstport);
        sk.addElement(key);
        pcap_stats.addPacket(key);
    }

    std::stringstream str_keys;
    str_keys.str(std::string());

    uint thr = 10081;

    //printing heavy hitters
    key_set_t heavy_keys = sk.getHeavyHitters(thr);
    if (heavy_keys.size() == 0)
        LOG(INFO) << "No heavy hitters";
    else {
        str_keys.str(std::string());
        for (key_set_t::iterator it = heavy_keys.begin();
             it != heavy_keys.end(); it++) {
            str_keys << *it << " ";
        }
        LOG(INFO) << "There are " << heavy_keys.size() << " heavy keys " << str_keys.str();
    }

    // actual stats
    str_keys.str(std::string());
    std::list<key> real_heavy_keys = pcap_stats.getFrequencies(thr);
    for (std::list<key>::iterator it = real_heavy_keys.begin();
            it != real_heavy_keys.end(); it++) {
        str_keys<<*it<< " ";
    }

    LOG(INFO) << sk.countElements() << " were processed";
    LOG(INFO) << "There are " << real_heavy_keys.size() << " real heavy hitters " << str_keys.str();
    LOG(INFO) << "There are " << pcap_stats.getDifferentPackets() << " different keys";
}

TEST_CASE( "Heavy Changers", "[sketch]" ) {

    pcap_parser pcap_file = pcap_parser("/home/leandro/CLionProjects/sketch/test/pcap-files/smallFlows.pcap");
    pcap_parser_stats pcap_stats;
    sketch sk1(5, 100, 9091);
    sketch sk2(5, 100, 9091);

    // reading pcap file
    uint n_packets_epoch = 200000;
    packet_info p;
    uint i = 0;
    while (pcap_file.has_next() and i < n_packets_epoch) {
        if (!pcap_file.next_packet(&p)) continue;
        in_addr_t _src = p.ip_src.s_addr;
        in_addr_t _dst = p.ip_dst.s_addr;
        key key(_src, p.srcport, _dst, p.dstport);
        sk1.addElement(key);
        pcap_stats.addPacket(key);
        i++;
    }

    i = 0;
    while (pcap_file.has_next() and i < n_packets_epoch) {
        if (!pcap_file.next_packet(&p)) continue;
        in_addr_t _src = p.ip_src.s_addr;
        in_addr_t _dst = p.ip_dst.s_addr;
        key key(_src, p.srcport, _dst, p.dstport);
        sk2.addElement(key);
        pcap_stats.addPacket(key);
        i++;
    }

    std::stringstream str_keys;
    str_keys.str(std::string());

    uint thr = 10081;

    //printing heavy hitters
    key_set_t heavy_changers = sk1.getHeavyChangers(sk2,thr);
    if (heavy_changers.size() == 0)
        LOG(INFO) << "No heavy changers";
    else {
        str_keys.str(std::string());
        for (key_set_t::iterator it = heavy_changers.begin();
             it != heavy_changers.end(); it++) {
            str_keys << *it << " ";
        }
        LOG(INFO) << "There are " << heavy_changers.size() << " heavy changers " << str_keys.str();
    }

    // actual stats
//    str_keys.str(std::string());
//    std::list<key> real_heavy_keys = pcap_stats.getFrequencies(thr);
//    for (std::list<key>::iterator it = real_heavy_keys.begin();
//         it != real_heavy_keys.end(); it++) {
//        str_keys<<*it<< " ";
//    }

    LOG(INFO) << sk1.countElements() + sk2.countElements() << " were processed";
//    LOG(INFO) << "There are " << real_heavy_keys.size() << " real heavy hitters " << str_keys.str();
    LOG(INFO) << "There are " << pcap_stats.getDifferentPackets() << " different keys";
}
