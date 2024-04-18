#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>  // Include for setw and setfill
#include <string>
#include <sstream>
#include "../cryptopp/keccak.h"
#include "../cryptopp/sha.h"
#include "../cryptopp/hmac.h"
#include "../cryptopp/ripemd.h"
#include "../cryptopp/blake2.h"

using namespace std;

class Hash {
public:
    Hash();

    static std::vector<uint8_t> hash(const std::vector<uint8_t>& input, std::string& algorithm);

    static std::vector<uint8_t> sha3(const std::vector<uint8_t>& input, int offset, int length);

    static vector<uint8_t> sha3(const vector<uint8_t>& input);

    static std::vector<uint8_t> sha256(const std::vector<uint8_t>& input);

    static std::vector<uint8_t> hmacSha512(const std::vector<uint8_t>& key, const std::vector<uint8_t>& input);

    static vector<uint8_t> sha256hash160(const vector<uint8_t>& input);

    static std::vector<uint8_t> blake2b256(const std::vector<uint8_t>& input);
};


