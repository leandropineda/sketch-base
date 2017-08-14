//
// Created by leandro on 19/07/17.
//

#ifndef SKETCH_TYPEDEFS_H
#define SKETCH_TYPEDEFS_H

#include <sys/types.h>
#include <vector>
#include <map>
#include <set>
#include <utility>

typedef uint32_t key;

class bucket;

typedef std::pair<uint32_t, uint32_t> hash_func_t;
typedef std::pair<key,uint32_t> event_pair_t;
typedef std::pair<uint32_t,uint32_t> estimation_t;
typedef std::map<key,uint32_t> event_counter_t;
typedef std::set<key> key_set_t;
typedef std::set<key> key_set_t;
typedef std::vector<bucket> bucket_list_t;

#endif //SKETCH_TYPEDEFS_H
