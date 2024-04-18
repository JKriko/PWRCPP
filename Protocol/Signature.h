#pragma once

#include <iostream>
//#include <integer.h>
//#include <ecp.h>
//#include <asn.h>
#include <vector>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/ecdh.h>
#include <openssl/obj_mac.h>
#include <openssl/sha.h>
#include <openssl/bn.h>
//#include <boost/multiprecision/cpp_dec_float.hpp>
#include "../Utils/Hash.h"

//namespace mp = boost::multiprecision;
//using BigDecimal = mp::number<mp::cpp_dec_float<50>>;
//using BigInteger = mp::cpp_int;

class ECDSASignature;

class Signature {
public:
    static EC_GROUP* curve;
    static const EC_POINT* G;
    static const BIGNUM* order;
    static const BIGNUM* cofactor;
    static BIGNUM* halfCurveOrder;

    static void init();

    static void cleanup();

    static vector<uint8_t> signMessage(const std::vector<uint8_t>& message, BIGNUM* privateKey);

    static ECDSASignature sign(BIGNUM* privateKey, vector<uint8_t> transactionHash);

    static EC_POINT* decompressKey(const BIGNUM* xBN, bool yBit);

    static BIGNUM* recoverFromSignature(int recId, const ECDSASignature& sig, const std::vector<uint8_t>& message);

    static BIGNUM* publicKeyFromPrivate(BIGNUM* privKey);

    static EC_POINT* publicPointFromPrivate(BIGNUM* privKey);

    static vector<uint8_t> toBytesPadded(const BIGNUM* value, int length);

    static void verifyPrecondition(bool assertionResult, const string& errorMessage);
};



class ECDSASignature {
public:
    const BIGNUM* r;
    const BIGNUM* s;

    ECDSASignature(const BIGNUM* r, const BIGNUM* s);

    bool isCanonical() const;

    ECDSASignature toCanonicalised() const;
};