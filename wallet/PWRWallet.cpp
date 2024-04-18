#include <iostream>
#include "PWRWallet.h"
#include "../Protocol/TransactionBuilder.h"


using namespace std;
namespace mp = boost::multiprecision;
using BigDecimal = mp::number<mp::cpp_dec_float<50>>;
using BigInteger = mp::cpp_int;
namespace rnd = boost::random;
using namespace std;


BigInteger PWRWallet::createPrivateKey(const std::vector<uint8_t>& privateKeyBytes) {
    // Initialize privateKey using byte array directly
    BigInteger key = 0;
    for (const auto& byte : privateKeyBytes) {
        key *= 256; // Equivalent to shifting left by 8 bits
        key += byte; // Add current byte
    }
    return key;
}

BIGNUM* PWRWallet:: BigIntegerToBIGNUM(BigInteger bigint) {
    // Convert the BigInteger to a hexadecimal string
    std::string hexString = bigint.str(16);

    // Convert the hexadecimal string to a BIGNUM
    BIGNUM* result = BN_new();
    if (!BN_hex2bn(&result, hexString.c_str())) {
        // Handle conversion failure
        BN_free(result); // Free the allocated BIGNUM
        return nullptr; // Return nullptr or throw an exception
    }

    return result;
}

BigInteger PWRWallet::BIGNUMToBigInteger(const BIGNUM* bn) {
    // Convert the BIGNUM to a hexadecimal string
    char* hexString = BN_bn2hex(bn);
    if (!hexString) {
        // Handle conversion failure
        return BigInteger(0); // Return default value or throw exception
    }

    // Convert the hexadecimal string to a BigInteger
    BigInteger result(hexString, 16);

    // Free the memory allocated for the hexadecimal string
    OPENSSL_free(hexString);

    return result;
}
    
BigInteger PWRWallet::generateRandomBigInt(){
    rnd::random_device rng;
    rnd::uniform_int_distribution<BigInteger> dist(0, (BigInteger(1) << 256) - 1);
    return dist(rng);
}

std::vector<uint8_t> PWRWallet::HexDecode(const std::string& hexString) {
    std::vector<uint8_t> bytes;

    for (size_t i = 0; i < hexString.length(); i += 2) {
        uint8_t byte;
        std::stringstream ss;
        ss << std::hex << hexString.substr(i, 2);
        ss >> byte;
        bytes.push_back(byte);
    }

    return bytes;
}  

std::vector<uint8_t> PWRWallet::stringToUtf8Bytes(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string utf32 = converter.from_bytes(str);
    std::vector<uint8_t> utf8_bytes(utf32.begin(), utf32.end());
    return utf8_bytes;
}

PWRWallet::PWRWallet(const string& privateKey, PWRJ pwrj)
   :privateKey(BigInteger(privateKey, 16)),
    pwrj(pwrj) {}

PWRWallet::PWRWallet(const std::vector<unsigned char>& privateKeyBytes, PWRJ pwrj) :
    privateKey(createPrivateKey(privateKeyBytes)),
    pwrj(pwrj) {}

PWRWallet::PWRWallet(BigInteger privateKey, PWRJ pwrj) :
    privateKey(privateKey),
    pwrj(pwrj) {}

PWRWallet::PWRWallet(PWRJ pwrj) : privateKey(generateRandomBigInt()), pwrj(pwrj) {}

string PWRWallet::getAddress() {
    BIGNUM* privateKeyBigNum = BigIntegerToBIGNUM(privateKey);
    return publicKeyToAddress(publicKeyFromPrivate(privateKeyBigNum));
}

string PWRWallet::publicKeyToAddress(BIGNUM* publicKeyBN) {
    // Convert BIGNUM* public key to byte array
    size_t keySize = BN_num_bytes(publicKeyBN);
    vector<unsigned char> publicKeyBytes(keySize);
    BN_bn2bin(publicKeyBN, publicKeyBytes.data());

    // If the first byte is 0x00 and the array is 65 bytes long, it is probably due to sign bit extension, and we can ignore it.
    if (publicKeyBytes.size() == 65 && publicKeyBytes[0] == 0) {
        publicKeyBytes.erase(publicKeyBytes.begin());
    }

    // Perform Keccak-256 hashing on the public key bytes
    CryptoPP::Keccak_256 hash;
    vector<unsigned char> addressBytes(32);
    hash.Update(publicKeyBytes.data(), publicKeyBytes.size());
    hash.Final(addressBytes.data());

    // Take the last 20 bytes of the hashed public key as the address bytes
    vector<unsigned char> addr(addressBytes.end() - 20, addressBytes.end());

    // Convert to hex string and prepend "0x"
    stringstream hexString;
    hexString << "0x";
    for (unsigned char byte : addr) {
        hexString << hex << setw(2) << setfill('0') << (int)byte;
    }
    return hexString.str();
}

long PWRWallet::getBalance() {
    try {
        return PWRJ::getBalanceOfAddress(getAddress());
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1; // Return a negative value to indicate error
    }
}

int PWRWallet::getNonce() {
    try {
        return PWRJ::getNonceOfAddress(getAddress());
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1; // Return a negative value to indicate error
    }
}

BigInteger PWRWallet::getPrivateKey() {
    return privateKey;
}

vector<uint8_t> PWRWallet::getSignedTxn(vector<uint8_t> txn){
    if (txn.empty()) return vector<uint8_t>();
    BIGNUM* privateKeyBigNum = PWRWallet::BigIntegerToBIGNUM(privateKey);
    // Sign the transaction
    std::vector<uint8_t> signature = Signature::signMessage(txn, privateKeyBigNum);

    // Allocate memory for the final transaction
    std::vector<uint8_t> finalTransaction(txn.size() + signature.size());

    // Copy transaction bytes
    copy(txn.begin(), txn.end(), finalTransaction.begin());

    // Append signature
    std::copy(signature.begin(), signature.end(), finalTransaction.begin() + txn.size());

    return finalTransaction;
}


vector<byte> PWRWallet::getSignedTransferPWRTxn(const string& to, long amount, int nonce) {
    try {
        return getSignedTxn(getTransferPWRTxn(to, amount, nonce));
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

Response PWRWallet::transferPWR(const string& to, long amount, int nonce) {
    try {
        return PWRJ::broadcastTxn(getSignedTransferPWRTxn(to, amount, nonce));
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

Response PWRWallet::transferPWR(const string& to, long amount) {
    try {
        return transferPWR(to, amount, getNonce());
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}


vector<uint8_t> PWRWallet::getSignedJoinTxn(const string& ip, int nonce) {
    try {
        return getSignedTxn(TransactionBuilder::getJoinTransaction(ip, nonce, PWRJ::getChainId()));
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

Response PWRWallet::join(const string& ip, int nonce) {
    try {
        return PWRJ::broadcastTxn(getSignedJoinTxn(ip, nonce));
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

Response PWRWallet::join(const string& ip) {
    try {
        return join(ip, getNonce());
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}


vector<uint8_t> PWRWallet::getSignedClaimActiveNodeSpotTxn(int nonce) {
    try {
        return getSignedTxn(TransactionBuilder::getClaimActiveNodeSpotTransaction(nonce, PWRJ::getChainId()));
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

Response PWRWallet::claimActiveNodeSpot(int nonce) {
    try {
        return PWRJ::broadcastTxn(getSignedClaimActiveNodeSpotTxn(nonce));
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

Response PWRWallet::claimActiveNodeSpot() {
    try {
        return claimActiveNodeSpot(getNonce());
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}


vector<uint8_t> PWRWallet::getSignedDelegateTxn(const string& validator, long amount, int nonce) {
    try {
        return getSignedTxn(TransactionBuilder::getDelegateTransaction(validator, amount, nonce, PWRJ::getChainId()));
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

Response PWRWallet::delegate(const string& to, long amount, int nonce) {
    try {
        return PWRJ::broadcastTxn(getSignedDelegateTxn(to, amount, nonce));
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

Response PWRWallet::delegate(const string& to, long amount) {
    try {
        return delegate(to, amount, getNonce());
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}


vector<uint8_t> PWRWallet::getSignedWithdrawTxn(const string& validator, long sharesAmount, int nonce) {
    try {
        return getSignedTxn(TransactionBuilder::getWithdrawTransaction(validator, sharesAmount, nonce, PWRJ::getChainId()));
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

Response PWRWallet::withdraw(const string& from, long sharesAmount, int nonce) {
    try {
        return PWRJ::broadcastTxn(getSignedWithdrawTxn(from, sharesAmount, nonce));
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

Response PWRWallet::withdraw(const string& from, long sharesAmount) {
    try {
        return withdraw(from, sharesAmount, getNonce());
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

vector<uint8_t> PWRWallet::getWithdrawPWRTxn(const string& from, long pwrAmount, int nonce) {
    try {
        if (from.length() != 40 && from.length() != 42) {
            throw std::runtime_error("Invalid address");
        }
        if (pwrAmount < 0) {
            throw std::runtime_error("Amount cannot be negative");
        }
        if (nonce < 0) {
            throw std::runtime_error("Nonce cannot be negative");
        }

        std::string normalizedTo = from;
        if (from.length() == 42) {
            normalizedTo = from.substr(2);
        }

        // Get share value from PWRJ class
        BigDecimal shareValue = PWRJ::getShareValue(from);

        // Perform the division
        BigDecimal pwrAmountDecimal(pwrAmount);
        BigDecimal sharesAmount = pwrAmountDecimal / shareValue;

        // Set precision and rounding mode
        sharesAmount.precision(18);
        sharesAmount = floor(sharesAmount);

        // Convert to long
        long longSharesAmount = sharesAmount.convert_to<long>();

        if (longSharesAmount <= 0) {
            throw std::runtime_error("Shares amount is too low");
        }

        vector<uint8_t> txnBase = getTxnBase(static_cast<uint8_t>(4), nonce);
        size_t bufferLength = txnBase.size() + 8 + normalizedTo.length() / 2;
        vector<uint8_t> buffer(bufferLength);

        // Copy txnBase to buffer
        std::memcpy(buffer.data(), txnBase.data(), txnBase.size());

        // Copy amount to buffer
        for (int i = sizeof(longSharesAmount) - 1; i >= 0; --i) {
            uint8_t byteValue = (longSharesAmount >> (8 * i)) & 0xFF;
            buffer.push_back(byteValue);
        }

        // Convert hexadecimal string to bytes and copy to buffer
        std::stringstream ss;
        for (size_t i = 0; i < normalizedTo.length(); i += 2) {
            uint8_t byte;
            ss << std::hex << normalizedTo.substr(i, 2);
            ss >> byte;
            buffer[txnBase.size() + sizeof(sharesAmount) + i / 2] = byte;
            ss.clear();
        }

        return buffer;
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

vector<uint8_t> PWRWallet::getSignedWithdrawPWRTxn(const string& from, long pwrAmount, int nonce) {
    try {
        return getSignedTxn(getWithdrawPWRTxn(from, pwrAmount, nonce));
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

Response PWRWallet::withdrawPWR(const string& from, long pwrAmount, int nonce) {
    try {
        return PWRJ::broadcastTxn(getSignedWithdrawPWRTxn(from, pwrAmount, nonce));
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

Response PWRWallet::withdrawPWR(const string& from, long pwrAmount) {
    try {
        return withdrawPWR(from, pwrAmount, getNonce());
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

vector<uint8_t> PWRWallet::getSignedVmDataTxn(long vmId, vector<uint8_t> data, int nonce) {
    try {
        return getSignedTxn(TransactionBuilder::getVmDataTransaction(vmId, data, nonce, PWRJ::getChainId()));
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

Response PWRWallet::sendVmDataTxn(long vmId, vector<uint8_t> data, int nonce) {
    try {
        return PWRJ::broadcastTxn(getSignedVmDataTxn(vmId, data, nonce));
    }
    catch (std::exception& e) {
        return Response(false, "", e.what());
    }
}

Response PWRWallet::sendVmDataTxn(long vmId, vector<uint8_t> data) {
    try {
        return sendVmDataTxn(vmId, data, getNonce());
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}


std::vector<std::uint8_t> PWRWallet::getSignedClaimVmIdTxn(long vmId, int nonce) {
    try {
        std::vector<std::uint8_t> txn = TransactionBuilder::getClaimVmIdTransaction(vmId, nonce, PWRJ::getChainId());
        return getSignedTxn(txn);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

Response PWRWallet::claimVmId(long vmId, int nonce) {
    try {
        return PWRJ::broadcastTxn(getSignedClaimVmIdTxn(vmId, nonce));
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

Response PWRWallet::claimVmId(long vmId) {
    try {
        return claimVmId(vmId, getNonce());
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}


std::vector<uint8_t> PWRWallet::getSignedSetGuardianTxn(const std::string& guardian, long expiryDate, int nonce) {
    try {
        return getSignedTxn(TransactionBuilder::getSetGuardianTransaction(guardian, expiryDate, nonce, PWRJ::getChainId()));
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

Response PWRWallet::setGuardian(const std::string& guardianAddress, long expiryDate, int nonce) {
    try {
        return PWRJ::broadcastTxn(getSignedSetGuardianTxn(guardianAddress, expiryDate, nonce));
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

Response PWRWallet::setGuardian(const std::string& guardianAddress, long expiryDate) {
    try {
        int nonce = getNonce();
        return setGuardian(guardianAddress, expiryDate, nonce);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

std::vector<uint8_t> PWRWallet::getSignedRemoveGuardianTxn(int nonce) {
    try {
        return getSignedTxn(TransactionBuilder::getRemoveGuardianTransaction(nonce, PWRJ::getChainId()));;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

Response PWRWallet::removeGuardian(int nonce) {
    return pwrj.broadcastTxn(getSignedRemoveGuardianTxn(nonce));
}

std::vector<uint8_t> PWRWallet::getSignedRemoveGuardianTxn() {
    try {
        return getSignedTxn(getRemoveGuardianTxn(getNonce()));
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

Response PWRWallet::removeGuardian(int nonce) {
    try {
        return PWRJ::broadcastTxn(getSignedRemoveGuardianTxn(nonce));
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return Response(false, "", e.what());
    }
}

Response PWRWallet::removeGuardian() {
    try {
        return removeGuardian(getNonce());
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        // Handle the error accordingly, return a default response or throw an exception
        return Response(false, "", e.what());
    }
}

std::vector<uint8_t> PWRWallet::getSignedGuardianApprovalTransaction(const vector<vector<uint8_t>>& txn, int nonce) {
    try {
        return getSignedTxn(TransactionBuilder::getGuardianApprovalTransaction(txn, nonce, PWRJ::getChainId()));
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

Response PWRWallet::sendGuardianApprovalTransaction(const vector<vector<uint8_t>>& txn, int nonce) {
    try {
        return PWRJ::broadcastTxn(getSignedGuardianApprovalTransaction(txn, nonce));
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return Response(false, "", e.what());
    }
}


std::vector<uint8_t> PWRWallet::getSignedPayableVmDataTxn(long vmId, long value, const std::vector<uint8_t>& data, int nonce) {
    try {
         return getSignedTxn(TransactionBuilder::getPayableVmDataTransaction(vmId, value, data, nonce, PWRJ::getChainId()));
         ;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

Response PWRWallet::sendPayableVmDataTxn(long vmId, long value, const std::vector<uint8_t>& data, int nonce) {
    try {
        return PWRJ::broadcastTxn(getSignedPayableVmDataTxn(vmId, value, data, nonce));
    }
    catch (const std::exception& e) {
        return Response(false, "", e.what());
    }
}

Response PWRWallet::sendPayableVmDataTxn(long vmId, long value, const std::vector<uint8_t>& data) {
    try {
 
        return sendPayableVmDataTxn(vmId, value, data, getNonce());
    }
    catch (const std::exception& e) {
        return Response(false, "", e.what());
    }
}

vector<uint8_t> PWRWallet::getSignedValidatorRemoveTxn(const std::string& validator, int nonce) {
    try {
        return getSignedTxn(TransactionBuilder::getValidatorRemoveTransaction(validator, nonce, PWRJ::getChainId()));
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

Response PWRWallet::sendValidatorRemoveTxn(const std::string& validator, int nonce) {
    try {
        return PWRJ::broadcastTxn(getSignedValidatorRemoveTxn(validator, nonce));;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

Response PWRWallet::sendValidatorRemoveTxn(const std::string& validator) {
    try {
        return PWRJ::broadcastTxn(getSignedValidatorRemoveTxn(validator, getNonce()));
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

std::vector<uint8_t> PWRWallet::getSignedConduitApprovalTxn(long vmId, const std::vector<std::vector<uint8_t>>& transactions, int nonce) {
    try {
        return getSignedTxn(TransactionBuilder::getConduitApprovalTransaction(vmId, transactions, nonce, PWRJ::getChainId()));
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

Response PWRWallet::conduitApprove(long vmId, const std::vector<std::vector<uint8_t>>& transactions, int nonce) {
    try {
        return PWRJ::broadcastTxn(getSignedConduitApprovalTxn(vmId, transactions, nonce));
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

Response PWRWallet::conduitApprove(long vmId, const std::vector<std::vector<uint8_t>>& transactions) {
    try {
        return conduitApprove(vmId, transactions, getNonce());
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

std::vector<uint8_t> PWRWallet::getSignedSetConduitTxn(long vmId, const vector<vector<uint8_t>>& conduits, int nonce) {
    try {
        return getSignedTxn(TransactionBuilder::getSetConduitsTransaction(vmId, conduits, nonce, PWRJ::getChainId()));
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

Response PWRWallet::setConduits(long vmId, const std::vector<std::vector<uint8_t>>& conduits, int nonce) {
    try {
        return PWRJ::broadcastTxn(getSignedSetConduitTxn(vmId, conduits, nonce));
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

Response PWRWallet::setConduits(long vmId, const std::vector<std::vector<uint8_t>>& conduits) {
    try {
        return setConduits(vmId, conduits, getNonce());
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

vector<uint8_t> PWRWallet::getSignedMoveStakeTransaction(long sharesAmount, const string& fromValidator, const string& toValidator, int nonce) {
    return getSignedTxn(TransactionBuilder::getMoveStakeTransaction(sharesAmount, fromValidator, toValidator, nonce, PWRJ::getChainId()));
}

Response PWRWallet::moveStake(long sharesAmount, const string& fromValidator, const string& toValidator, int nonce) {
    return PWRJ::broadcastTxn(getSignedMoveStakeTransaction(sharesAmount, fromValidator, toValidator, nonce));
}

BIGNUM* PWRWallet::publicKeyFromPrivate(BIGNUM* privKey) {
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

EC_POINT* PWRWallet::publicPointFromPrivate(BIGNUM* privKey) {
    if (BN_num_bits(privKey) > BN_num_bits(Signature::order)) {
        BIGNUM* reducedPrivKey = BN_new();
        BN_mod(reducedPrivKey, privKey, EC_GROUP_get0_order(Signature::curve), NULL);
        privKey = reducedPrivKey;
    }

    EC_POINT* publicKey = EC_POINT_new(Signature::curve);
    EC_POINT_mul(Signature::curve, publicKey, privKey, NULL, NULL, NULL);

    BN_free(privKey);
    return publicKey;
};
