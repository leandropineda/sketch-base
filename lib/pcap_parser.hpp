//
// Created by leandro on 05/07/17.
//

#ifndef SKETCH_PCAP_PARSER_HPP
#define SKETCH_PCAP_PARSER_HPP

#include <pcap/pcap.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <errno.h>
#include <cstring>
#include <list>
#include "easylogging++.h"


/* DEFINING STRUCTURES FOR PACKET READING */
/* Ethernet addresses are 6 bytes */
#define ETHER_ADDR_LEN	6

/* Ethernet header */
struct sniff_ethernet {
    u_char ether_dhost[ETHER_ADDR_LEN]; /* Destination host address */
    u_char ether_shost[ETHER_ADDR_LEN]; /* Source host address */
    u_short ether_type; /* IP? ARP? RARP? etc */
};

/* IP header */
struct sniff_ip {
    u_char ip_vhl;		/* version << 4 | header length >> 2 */
    u_char ip_tos;		/* type of service */
    u_short ip_len;		/* total length */
    u_short ip_id;		/* identification */
    u_short ip_off;		/* fragment offset field */
    #define IP_RF 0x8000		/* reserved fragment flag */
    #define IP_DF 0x4000		/* dont fragment flag */
    #define IP_MF 0x2000		/* more fragments flag */
    #define IP_OFFMASK 0x1fff	/* mask for fragmenting bits */
    u_char ip_ttl;		/* time to live */
    u_char ip_p;		/* protocol */
    u_short ip_sum;		/* checksum */
    struct in_addr ip_src, ip_dst; /* source and dest address */
};

#define IP_HL(ip)		(((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)		(((ip)->ip_vhl) >> 4)

/* TCP header */
typedef u_int tcp_seq;

struct sniff_tcp {
    u_short th_sport;	/* source port */
    u_short th_dport;	/* destination port */
    tcp_seq th_seq;		/* sequence number */
    tcp_seq th_ack;		/* acknowledgement number */
    u_char th_offx2;	/* data offset, rsvd */
    #define TH_OFF(th)	(((th)->th_offx2 & 0xf0) >> 4)
    u_char th_flags;
    #define TH_FIN 0x01
    #define TH_SYN 0x02
    #define TH_RST 0x04
    #define TH_PUSH 0x08
    #define TH_ACK 0x10
    #define TH_URG 0x20
    #define TH_ECE 0x40
    #define TH_CWR 0x80
    #define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
    u_short th_win;		/* window */
    u_short th_sum;		/* checksum */
    u_short th_urp;		/* urgent pointer */
};

/* UDP protocol header. */
struct sniff_udp {
    u_short uh_sport;               /* source port */
    u_short uh_dport;               /* destination port */
    u_short uh_ulen;                /* udp length */
    u_short uh_sum;                 /* udp checksum */
};

#define SIZE_ETHERNET 14

struct packet_info {
    struct timeval ts;	/* time stamp */
    struct in_addr ip_src, ip_dst;
    u_short srcport, dstport;
    u_char ip_p;		/* protocol */
};

class pcap_parser {

private:
    FILE * pFile;

    pcap_t* pcap_fh;        /* pcap file handler */
    int pcap_state, n_packets, n_err;

    void open_file(std::string path) {
        LOG(INFO) << "Opening pcap capture file";
        char errbuff[PCAP_ERRBUF_SIZE];

        pFile = fopen (path.c_str(), "r");
        if (pFile == NULL) LOG(ERROR) << "Failed to open "<< path.c_str() << " with error " << strerror(errno);

        this->pcap_fh = pcap_fopen_offline(pFile, errbuff);
        if (this->pcap_fh== NULL) LOG(ERROR) << "Error opening pcap file " << errbuff;

        LOG(INFO) << "Done (pointer " << this->pcap_fh << ")";

    }

public:
    pcap_parser(std::string path) {
        this->open_file(path);
        n_packets = 0;
        n_err = 0;
    }

    ~pcap_parser() {
        pcap_close(this->pcap_fh);
        LOG(INFO) << "Processed " << n_packets + n_err << " with " << n_err << " errors";
    }

    bool has_next() {
        return this->pcap_state >= 0;
    }

    bool next_packet(packet_info* p_info) {
        struct pcap_pkthdr *header;	/* The header that pcap gives us */
        const u_char *packet;		/* The actual packet */

        this->pcap_state = pcap_next_ex(pcap_fh, &header, &packet);

        if (!this->has_next()) return false;

        //const struct sniff_ethernet *ethernet; /* The ethernet header */
        //ethernet = (struct sniff_ethernet*)(packet);

        p_info->ts = header->ts;

        const struct sniff_ip *ip; /* The IP header */
        ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);

        int size_ip = IP_HL(ip) * 4;

        if (size_ip < 20) {
            LOG(WARNING) << "Invalid IP header length: " << size_ip << " bytes";
            this->n_err++;
            return false;
        }

        if (IP_V(ip) == 6) {
            LOG(WARNING) << "IPv6 packet";
            this->n_err++;
            return false;
        }

        p_info->ip_src = ip->ip_src;
        p_info->ip_dst = ip->ip_dst;

        if (ip->ip_p == 6) {

            const struct sniff_tcp *tcp; /* The TCP header */
            tcp = (struct sniff_tcp *) (packet + SIZE_ETHERNET + size_ip);

            p_info->srcport = tcp->th_sport;
            p_info->dstport = tcp->th_dport;

            int size_tcp = TH_OFF(tcp) * 4;
            if (size_tcp < 20) {
                LOG(WARNING) << "Invalid TCP header length: " << size_tcp << " bytes";
                this->n_err++;
                return false;
            }

        }

        if (ip->ip_p == 17) {
            const struct sniff_udp *udp; /* The UDP header */
            udp = (struct sniff_udp *) (packet + SIZE_ETHERNET + size_ip);

            int size_udp = ntohs(udp->uh_ulen);

            if (size_udp < 20) {
                LOG(WARNING) << "Invalid UDP header length: " << size_udp << " bytes";
                this->n_err++;
                return false;
            }

            p_info->srcport = udp->uh_sport;
            p_info->dstport = udp->uh_dport;

        }

        p_info->ip_p = ip->ip_p;
        this->n_packets++;
        return true;
    }
};


typedef std::map<key, uint> packet_uint_map_t;
typedef std::multimap<uint, key> uint_keys_mmap_t;

class pcap_parser_stats {
private:
    packet_uint_map_t packet_map;
public:
    pcap_parser_stats() {
        this->packet_map.clear();
    }

    void addPacket(key &k) {
        packet_uint_map_t::iterator it = packet_map.find(k);
        if (it != packet_map.end()) it->second++;
        else packet_map.insert(std::make_pair(k, 1));
    }

    ulong getDifferentPackets() {
        return this->packet_map.size();
    }

    std::list<key> getFrequencies(uint threshold = 0) {
        uint_keys_mmap_t keys_by_value;
        for (packet_uint_map_t::iterator it = packet_map.begin();
                it != packet_map.end(); it++) {
//            LOG(DEBUG) << "Inserting " << it->second << " " << it->first;
            keys_by_value.insert(std::make_pair(it->second, it->first));
        }

        std::list<key> keys;
        uint_keys_mmap_t::reverse_iterator rit = keys_by_value.rbegin();
        while (rit != keys_by_value.rend() and rit->first > threshold) {
            keys.push_back(rit->second);
            ++rit;
        }

        return keys;

    }
};

#endif //SKETCH_PCAP_PARSER_HPP
