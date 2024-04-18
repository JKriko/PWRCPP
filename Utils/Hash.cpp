#include "Hash.h"

//using namespace Hash;

using namespace std;

Hash::Hash() {}

std::vector<uint8_t> Hash::hash(const std::vector<uint8_t>& input, std::string& algorithm) {

    for (char& c : algorithm) {
        c = std::toupper(c);
    }

    if (algorithm == "SHA-256") {
        return Hash::sha256(input);
    }
    else if (algorithm == "SHA3-256") {
        return Hash::sha3(input);
    }
    else if (algorithm == "HMACSHA512") {
        // Replace 'your_key_here' with your actual key
        std::vector<uint8_t> key = { 'y', 'o', 'u', 'r', '_', 'k', 'e', 'y', '_', 'h', 'e', 'r', 'e' };
        return Hash::hmacSha512(key, input);
    }
    else if (algorithm == "RIPEMD160") {
        return Hash::sha256hash160(input);
    }
    else if (algorithm == "BLAKE2B-256") {
        return Hash::blake2b256(input);
    }
    else {
        throw std::runtime_error("Unsupported hash algorithm: " + algorithm);
    }
}

std::vector<uint8_t> Hash::sha3(const std::vector<uint8_t>& input, int offset, int length) {
    CryptoPP::Keccak_256 keccak;
    keccak.Update(input.data() + offset, length);
    std::vector<uint8_t> result(keccak.DigestSize());
    keccak.Final(result.data());
    return result;
}

 vector<uint8_t> Hash::sha3(const vector<uint8_t>& input) {
    return Hash::sha3(input, 0, input.size());
}

std::vector<uint8_t> Hash::sha256(const std::vector<uint8_t>& input) {
    try {
        CryptoPP::SHA256 sha256;
        sha256.Update(input.data(), input.size());
        std::vector<uint8_t> result(sha256.DigestSize());
        sha256.Final(result.data());
        return result;
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Couldn't find a SHA-256 provider: " + std::string(e.what()));
    };
}

std::vector<uint8_t> Hash::hmacSha512(const std::vector<uint8_t>& key, const std::vector<uint8_t>& input) {
    CryptoPP::HMAC<CryptoPP::SHA512> hmac(key.data(), key.size());
    hmac.Update(input.data(), input.size());
    std::vector<uint8_t> result(hmac.DigestSize());
    hmac.Final(result.data());
    return result;
}

vector<uint8_t> Hash::sha256hash160(const vector<uint8_t>& input) {
    vector<uint8_t> sha_256 = sha256(input);
    CryptoPP::RIPEMD160 ripemd160;
    ripemd160.Update(sha_256.data(), sha_256.size());
    vector<uint8_t> result(ripemd160.DigestSize());
    ripemd160.Final(result.data());
    return result;
}

std::vector<uint8_t> Hash::blake2b256(const std::vector<uint8_t>& input) {
    CryptoPP::BLAKE2b blake2b; // Initialize BLAKE2b object
    std::vector<uint8_t> hash(CryptoPP::BLAKE2b::DIGESTSIZE); // Allocate a byte vector for the full hash output
    blake2b.Update(input.data(), input.size()); // Update hash with input data
    blake2b.Final(hash.data()); // Finalize hash computation

    // Truncate the hash to 32 bytes (256 bits)
    std::vector<uint8_t> truncatedHash(32);
    std::copy(hash.begin(), hash.begin() + 32, truncatedHash.begin());

    return truncatedHash;
}


//int main() {
//    std::vector<uint8_t> input = { 'h', 'e', 'l', 'l', 'o' }; // Example input
//
//    std::vector<uint8_t> output = Hash::blake2b256(input);
//
//    std::cout << "Input: ";
//    for (auto& byte : input) {
//        std::cout << byte;
//    }
//    std::cout << std::endl;
//
//    std::cout << "Output (BLAKE2b-256): ";
//    for (auto& byte : output) {
//        std::cout << std::hex << static_cast<int>(byte);
//    }
//    std::cout << std::endl;
//
//    return 0;
//}

//int main() {
//    std::vector<uint8_t> input = { 'h', 'e', 'l', 'l', 'o' }; // Example input
//    int offset = 0; // Start from the beginning of the input
//    int length = input.size(); // Consider the entire input
//
//    std::vector<uint8_t> output = Hash::sha3(input, offset, length);
//
//    std::cout << "Input: ";
//    for (auto& byte : input) {
//        std::cout << byte;
//    }
//    std::cout << std::endl;
//
//    std::cout << "Output (SHA3-256): ";
//    for (auto& byte : output) {
//        std::cout << std::hex << static_cast<int>(byte);
//    }
//    std::cout << std::endl;
//
//    return 0;
//}

//int main() {
//    cout << "Hello World" << endl;
//}