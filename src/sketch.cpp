//
// Created by leandro on 17/06/17.
//

#include "sketch.h"

sketch::sketch(uint d, uint w, uint p, uint max_dict_length) : d(d), w(w), p(p), max_dict_length(max_dict_length) {
    srand(time(NULL));
    // hash functions are defined as
    //   h_ab(key) = ((a * key + b) mod p) mod w
    // where:
    //   w is the number of buckets
    //   p is a prime and p >= w
    //   1 <= a <= p-1 and
    //   0 <= b <= p-1 are chosen randomly
    assert(this->p >= this->w); //universal hashing
    assert(this->__isPrime(this->p));
    LOG(INFO) << "Sketch: buckets " << this->w << ", "
              << "hash functions " << this->d << ", "
              << "parameter p " << this->p;

    // make sure start without any function defined
    this->hash_func.clear();

    this->__allocSketch();

    // set d hash functions
    this->__buildHashFunc();
}

void sketch::addElement(key &k) {
    assert (this->hash_func.size());
    //LOG(DEBUG) << "Adding element " << k << " to sketch";
    for (uint i = 0; i < this->d; i++) {
        uint col = this->__hashKey(this->hash_func[i].first, this->hash_func[i].second, k);
        this->matrix->at(this->__index(i, col)).addElement(k);
        //LOG(DEBUG) << "Hashing to " << i << ", " << col << ". Element " << this->matrix->at(this->__index(i, col));
    }
}

uint sketch::__index(uint i, uint j) const {
    return (i*w)+j;
}

uint sketch::__hashKey(uint a, uint b, key &k) {
    return ((a*k + b) % this->p) % this->w;
}

void sketch::__buildHashFunc() {

    LOG(INFO) << "Building hash function(s)";

    assert(!this->hash_func.size());

    while(hash_func.size() < d) {

        hash_func_t h = hash_func_t((rand() % (this->p - 2)) + 1,
                                    (rand() % (this->p - 1)));

        LOG(DEBUG) << "Hash function: " << "(" << h.first << "," << h.second << ")";
        this->hash_func.push_back(h);

        std::set<hash_func_t> unique_hash_func(hash_func.begin(), hash_func.end());
        if (hash_func.size() > unique_hash_func.size()) {
            LOG(WARNING) << "Found function duplicate. Removing";
            this->hash_func = std::vector<hash_func_t>(unique_hash_func.begin(), unique_hash_func.end());
        }
    }

    LOG(INFO) << "Done: " << hash_func.size() << " function(s) has been built";
}

std::ostream &operator<<(std::ostream &os, const sketch &sk) {
    for (uint i = 0; i < (sk.d*sk.w); i++) {
            os << std::setw(2) << "E: " + std::to_string(i) + " "
               << sk.matrix->at(i) << std::endl;
        }

    return os;
}

bool sketch::__isPrime(uint n) const {
    if (n <= 1) return false;
    else if (n <= 3) return true;
        else if ((n % 2) == 0 or (n % 3) == 0) return false;
    uint i = 5;
    while (i * i <= n) {
        if (n % i == 0 or n % (i + 2) == 0) return false;
        i = i + 6;
    }
    return true;
}

bool sketch::__doesTheKeyExceedsTheGivenThresholdOnAllBuckets(key &k, uint threshold) {
    for (uint row = 0; row < this->d; row++) {
        uint col = this->__hashKey(this->hash_func.at(row).first,
                                   this->hash_func.at(row).second, k);
        if (this->matrix->at(this->__index(row,col)).estimateElementFrequency(k).second < threshold) {
            return false;
        }
    }
    return true;
}

bool sketch::__doesTheDifferenceBetweenFrequencyEstimationForTheKeyExceedsTheGivenThresholdOnAllBuckets(key &k, sketch sk, uint threshold) {
    estimation_t e1, e2;

    for (uint row = 0; row < this->d; row++) {
        uint col = this->__hashKey(this->hash_func.at(row).first,
                                   this->hash_func.at(row).second, k);
        // The key hashes to the same bucket on both sketches?
        e1 = this->matrix->at(this->__index(row,col)).estimateElementFrequency(k);
        e2 = sk.matrix->at(this->__index(row,col)).estimateElementFrequency(k);

        uint diffBetweenSketchesForTheKey_k = std::max(e1.second - e2.first, e2.second - e1.first);
        if (diffBetweenSketchesForTheKey_k < threshold) {
            return false;
        }
    }
    return true;
}

uint sketch::countElements() {
    uint elements = 0;
    for (uint i = 0; i < this->w; i++) {
        elements += this->matrix->at(i).getCounterValue();
    }
    return elements;
}

key_set_t sketch::getHeavyHitters(uint threshold) {
    key_set_t key_set;
    for (bucket_list_t::iterator it = this->matrix->begin(); it != this->matrix->end(); it++) {
        bucket bucket = *it;
        if (bucket.getCounterValue() > threshold) {
            key_set_t bucket_keys = bucket.getKeySet();
            for (key_set_t::iterator set_it = bucket_keys.begin();
                    set_it != bucket_keys.end(); set_it++) {
                key k = *set_it;
                if (this->__doesTheKeyExceedsTheGivenThresholdOnAllBuckets(k, threshold))
                    key_set.insert(k);
            }
        }
    }

    return key_set;
}

sketch::~sketch() {
}

key_set_t sketch::getHeavyChangers(sketch &sk, uint threshold) {
    assert(this->matrix->size() == sk.matrix->size());

    key_set_t keysOnBucketsExceedingThreshold;
    for (uint i = 0; i < this->matrix->size(); i++) {
        bucket b1 = this->matrix->at(i);
        bucket b2 = sk.matrix->at(i);
        if (b1.getCounterValue() > threshold or
                b2.getCounterValue() > threshold) {
            key_set_t keys;
            keys = b1.getKeySet();
            keysOnBucketsExceedingThreshold.insert(keys.begin(), keys.end());
            keys = b2.getKeySet();
            keysOnBucketsExceedingThreshold.insert(keys.begin(), keys.end());
        }
    }

    key_set_t heavyChangers;
    for (key_set_t::iterator it = keysOnBucketsExceedingThreshold.begin();
            it != keysOnBucketsExceedingThreshold.end(); it++) {
        key k = *it;
        if (this->__doesTheDifferenceBetweenFrequencyEstimationForTheKeyExceedsTheGivenThresholdOnAllBuckets(k,sk,threshold))
            heavyChangers.insert(k);

    }

    return heavyChangers;
}

void sketch::clear() {
    delete[] this->matrix;
    this->__allocSketch();
}

void sketch::__allocSketch() {
    // allocate the sketch on memory
    // bucket default constructor must initialize empty buckets
    this->matrix = new bucket_list_t;
    for (uint i = 0; i < (w*d); i++) {
        this->matrix->push_back(bucket(max_dict_length));
    }
}




