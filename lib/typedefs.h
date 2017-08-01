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

typedef uint key;

class bucket;

typedef std::pair<uint, uint> hash_func_t;
typedef std::pair<key,uint> event_pair_t;
typedef std::pair<uint,uint> estimation_t;
typedef std::map<key,uint> event_counter_t;
typedef std::set<key> key_set_t;
typedef std::set<key> key_set_t;
typedef std::vector<bucket> bucket_list_t;

#endif //SKETCH_TYPEDEFS_H
