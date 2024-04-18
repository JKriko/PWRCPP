#include "Signature.h"

EC_GROUP* Signature::curve = nullptr;
const EC_POINT* Signature::G = nullptr;
const BIGNUM* Signature::order = nullptr;
const BIGNUM* Signature::cofactor = nullptr;
BIGNUM* Signature::halfCurveOrder = nullptr;

// Initialize static members
void Signature::init() {
    curve = EC_GROUP_new_by_curve_name(NID_secp256k1);
    G = EC_GROUP_get0_generator(curve);
    order = EC_GROUP_get0_order(curve);
    cofactor = EC_GROUP_get0_cofactor(curve);
    halfCurveOrder = BN_new();
    BN_rshift1(halfCurveOrder, order);

    // Ensure that the curve and generator are properly initialized
    if (curve == nullptr || G == nullptr || order == nullptr || cofactor == nullptr || halfCurveOrder == nullptr) {
        // Handle initialization failure
    }
}

// Cleanup static members
void Signature::cleanup() {
    EC_GROUP_free(curve);
    BN_free(halfCurveOrder);
}

vector<uint8_t> Signature::signMessage(const std::vector<uint8_t>& message, BIGNUM* privateKey) {
    BIGNUM* publicKey = Signature::publicKeyFromPrivate(privateKey);
    std::vector<uint8_t> messageHash = Hash::sha3(message);

    ECDSASignature sig = Signature::sign(privateKey, messageHash);
    int recId = -1;

    int headerByte;

    for (headerByte = 0; headerByte < 4; ++headerByte) {
        BIGNUM* k = Signature::recoverFromSignature(headerByte, sig, messageHash);
        if (!k == 0 && k == publicKey) {
            recId = headerByte;
            break;
        }
    }

    if (recId == -1) {
        throw std::runtime_error("Could not construct a recoverable key. Are your credentials valid?");
    }
    else {
        headerByte = recId + 27;
        vector<uint8_t> v(1, static_cast<uint8_t>(headerByte));
        vector<uint8_t> r = toBytesPadded(sig.r, 32);
        vector<uint8_t> s = toBytesPadded(sig.s, 32);

        vector<uint8_t> signature(65);
        //r,s,v
        std::copy(r.begin(), r.end(), signature.begin());
        std::copy(s.begin(), s.end(), signature.begin() + 32);
        std::copy(v.begin(), v.end(), signature.begin() + 64);

        return signature;
    }
};


BIGNUM* Signature::recoverFromSignature(int recId, const ECDSASignature& sig, const std::vector<uint8_t>& message) {
    // Verify preconditions
    verifyPrecondition(recId >= 0, "recId must be non-negative");
    verifyPrecondition(BN_cmp(sig.r, BN_value_one()) >= 0, "r must be positive");
    verifyPrecondition(BN_cmp(sig.s, BN_value_one()) >= 0, "s must be positive");
    verifyPrecondition(!message.empty(), "message cannot be empty");

    // Calculate i
    BIGNUM* i = BN_new();
    BN_set_word(i, static_cast<long>(recId / 2L));

    // Calculate x
    BIGNUM* x = BN_new();
    BN_mul(x, i, order, NULL);
    BN_add(x, x, sig.r);

    // Ensure x is within bounds
    if (BN_cmp(x, order) >= 0) {
        BN_free(i);
        BN_free(x);
        return nullptr;
    }

    // Decompress key
    EC_POINT* R = decompressKey(x, recId & 1);

    // Check if R is valid
    if (!EC_POINT_mul(curve, R, NULL, R, order, NULL)) {
        BN_free(i);
        BN_free(x);
        EC_POINT_free(R);
        return nullptr;
    }

    // Calculate e
    BIGNUM* e = BN_bin2bn(message.data(), message.size(), NULL);

    // Calculate eInv and rInv
    BIGNUM* eInv = BN_new();
    BN_sub(eInv, BN_value_one(), e);
    BN_mod(eInv, eInv, order, NULL);
    BIGNUM* rInv = BN_mod_inverse(NULL, sig.r, order, NULL);

    // Calculate srInv and eInvrInv
    BIGNUM* srInv = BN_new();
    BN_mod_mul(srInv, sig.s, rInv, order, NULL);
    BIGNUM* eInvrInv = BN_new();
    BN_mod_mul(eInvrInv, rInv, eInv, order, NULL);

    // Calculate q
    EC_POINT* q = EC_POINT_new(curve);
    EC_POINT* tmp1 = EC_POINT_new(curve);
    EC_POINT* tmp2 = EC_POINT_new(curve);
    EC_POINT_mul(curve, tmp1, NULL, G, eInvrInv, NULL);
    EC_POINT_mul(curve, tmp2, NULL, R, srInv, NULL);
    EC_POINT_add(curve, q, tmp1, tmp2, NULL);

    // Get x-coordinate of q
    BIGNUM* qx = BN_new();
    EC_POINT_get_affine_coordinates(curve, q, qx, NULL, NULL);

    // Cleanup
    BN_free(i);
    BN_free(x);
    BN_free(e);
    BN_free(eInv);
    BN_free(rInv);
    BN_free(srInv);
    BN_free(eInvrInv);
    EC_POINT_free(R);
    EC_POINT_free(q);
    EC_POINT_free(tmp1);
    EC_POINT_free(tmp2);

    return qx;
}

ECDSASignature Signature::sign(BIGNUM* privateKey, vector<uint8_t> transactionHash) {
    // Create an EC_KEY object and set the private key
    EC_KEY* ecKey = EC_KEY_new();
    if (!ecKey) {
        throw std::runtime_error("Failed to create EC_KEY object");
    }
    if (!EC_KEY_set_group(ecKey, curve)) {
        EC_KEY_free(ecKey);
        throw std::runtime_error("Failed to set curve for EC_KEY object");
    }
    if (!EC_KEY_set_private_key(ecKey, privateKey)) {
        EC_KEY_free(ecKey);
        throw std::runtime_error("Failed to set private key");
    }

    ECDSA_SIG* sig = ECDSA_do_sign(transactionHash.data(), transactionHash.size(), ecKey);
    if (!sig) {
        EC_KEY_free(ecKey);
        throw std::runtime_error("Failed to generate ECDSA signature");
    }
    // Create BIGNUM objects to store the signature components
    BIGNUM* r = BN_new();
    BIGNUM* s = BN_new();

    // Temporary const BIGNUM* pointers
    const BIGNUM* temp_r;
    const BIGNUM* temp_s;

    // Extract the r and s components from the ECDSA signature
    ECDSA_SIG_get0(sig, &temp_r, &temp_s);

    // Copy the values to the non-const BIGNUM pointers
    BN_copy(r, temp_r);
    BN_copy(s, temp_s);

    // Free the ECDSA signature object
    ECDSA_SIG_free(sig);

    // Free the EC_KEY object
    EC_KEY_free(ecKey);

    // Create and return the ECDSASignature object
    return ECDSASignature(r, s).toCanonicalised();
}

EC_POINT* Signature::decompressKey(const BIGNUM* xBN, bool yBit) {
    size_t xByteLength = BN_num_bytes(xBN);
    unsigned char* xBytes = (unsigned char*)malloc(xByteLength);
    if (!xBytes) {
        return nullptr;
    }
    BN_bn2bin(xBN, xBytes);

    unsigned char pointEncoding = yBit ? 0x03 : 0x02;

    std::vector<unsigned char> compEnc(1 + xByteLength);
    compEnc[0] = pointEncoding;
    std::copy(xBytes, xBytes + xByteLength, compEnc.begin() + 1);

    EC_POINT* decompressedPoint = EC_POINT_new(Signature::curve);
    if (!decompressedPoint) {
        free(xBytes);
        return nullptr;
    }

    if (!EC_POINT_oct2point(Signature::curve, decompressedPoint, compEnc.data(), compEnc.size(), NULL)) {
        free(xBytes);
        EC_POINT_free(decompressedPoint);
        return nullptr;
    }

    free(xBytes);

    return decompressedPoint;
}

BIGNUM* Signature::publicKeyFromPrivate(BIGNUM* privKey) {
    EC_POINT* point = Signature::publicPointFromPrivate(privKey);
    size_t size = EC_POINT_point2oct(Signature::curve, point, POINT_CONVERSION_UNCOMPRESSED, NULL, 0, NULL);
    std::vector<unsigned char> encoded(size);
    if (EC_POINT_point2oct(Signature::curve, point, POINT_CONVERSION_UNCOMPRESSED, encoded.data(), size, NULL) != size) {
        EC_POINT_free(point);
        EC_GROUP_free(Signature::curve);
        throw std::runtime_error("Failed to encode public key point");
    }

    // Create a new BIGNUM and convert the remaining bytes to it
    BIGNUM* publicKeyBN = BN_new();
    if (!publicKeyBN) {
        throw std::runtime_error("Failed to create BIGNUM for public key");
    }

    if (!BN_bin2bn(encoded.data(), encoded.size(), publicKeyBN)) {
        BN_free(publicKeyBN);
        throw std::runtime_error("Failed to convert public key bytes to BIGNUM");
    }

    return publicKeyBN;
}

EC_POINT* Signature::publicPointFromPrivate(BIGNUM* privKey) {
    if (BN_num_bits(privKey) > BN_num_bits(order)) {
        BIGNUM* reducedPrivKey = BN_new();
        BN_mod(reducedPrivKey, privKey, EC_GROUP_get0_order(curve), NULL);
        privKey = reducedPrivKey;
    }

    EC_POINT* publicKey = EC_POINT_new(curve);
    EC_POINT_mul(curve, publicKey, privKey, NULL, NULL, NULL);

    BN_free(privKey);
    return publicKey;
};

vector<uint8_t> Signature::toBytesPadded(const BIGNUM* value, int length) {
    std::vector<uint8_t> result(length);
    std::vector<uint8_t> bytes(length); // Buffer to store the byte representation
    int bytesLength;
    int srcOffset;

    // Convert BIGNUM to bytes
    int actualBytesLength = BN_num_bytes(value);
    BN_bn2bin(value, bytes.data());

    // Determine the offset and length of the byte representation
    if (bytes[0] == 0) {
        bytesLength = actualBytesLength - 1;
        srcOffset = 1;
    }
    else {
        bytesLength = actualBytesLength;
        srcOffset = 0;
    }

    // Check if the byte representation exceeds the target length
    if (bytesLength > length) {
        throw std::runtime_error("Input is too large to put in byte array of size " + std::to_string(length));
    }
    else {
        // Calculate the offset to copy bytes to the result
        int destOffset = length - bytesLength;
        // Copy bytes to the result with padding
        std::copy(bytes.begin() + srcOffset, bytes.end(), result.begin() + destOffset);
        return result;
    }
}

void Signature::verifyPrecondition(bool assertionResult, const string& errorMessage) {
    if (!assertionResult) {
        throw std::runtime_error(errorMessage);
    }
}
////////////////////////////////////////////////////////////

ECDSASignature::ECDSASignature(const BIGNUM* r, const BIGNUM* s) : r(r), s(s) {}

bool ECDSASignature::isCanonical() const {
    bool result = BN_cmp(s, Signature::halfCurveOrder) <= 0;
    return result;
}

ECDSASignature ECDSASignature::toCanonicalised() const {
    if (!isCanonical()) {
        BIGNUM* newS = BN_new();
        BN_sub(newS, Signature::order, s);
        return ECDSASignature(r, newS);
    }
    return *this;
}