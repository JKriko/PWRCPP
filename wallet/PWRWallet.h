#include <iostream>
#include <string>
#include <vector>
#include <cstddef> // Include for std::byte
#include <codecvt>
//#include "boost/multiprecision/cpp_int.hpp"
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include "../cryptopp/hex.h"
#include <../cryptopp/keccak.h>
#include "../Protocol/PWRJ.h"
#include <openssl/bn.h>
//#include "boost/multiprecision/cpp_bin_float.hpp"
#include <boost/multiprecision/cpp_dec_float.hpp>
#include "../Protocol/Signature.h"


using namespace std;
namespace mp = boost::multiprecision;
using BigDecimal = mp::number<mp::cpp_dec_float<50>>;
using BigInteger = mp::cpp_int;
namespace rnd = boost::random;


class PWRWallet {
private:
    static httplib::Client client;
    BigInteger privateKey;
    PWRJ pwrj;

    BigInteger createPrivateKey(const std::vector<unsigned char>& privateKeyBytes);

    BigInteger generateRandomBigInt();

    std::vector<uint8_t> HexDecode(const std::string& hexString);

    std::vector<uint8_t> stringToUtf8Bytes(const std::string& str);

    BIGNUM* BigIntegerToBIGNUM(BigInteger bigint);

    BigInteger BIGNUMToBigInteger(const BIGNUM* bn);

public:
    PWRWallet(const string& privateKey, PWRJ pwrj);

    PWRWallet(const std::vector<unsigned char>& privateKeyBytes, PWRJ pwrj);

    PWRWallet(BigInteger privateKey, PWRJ pwrj);

    PWRWallet(PWRJ pwrj);

    string getAddress();

    static string publicKeyToAddress(BIGNUM* publicKey);

    long getBalance();

    int getNonce();

    BigInteger getPrivateKey();

    vector<uint8_t> getSignedTxn(vector<uint8_t> txn);

    std::vector<uint8_t> getTxnBase(uint8_t identifier, int nonce);

    std::vector<uint8_t> getTransferPWRTxn(std::string to, long amount, int nonce);

    vector<byte> getSignedTransferPWRTxn(const string& to, long amount, int nonce);

    Response transferPWR(const string& to, long amount, int nonce);

    Response transferPWR(const string& to, long amount);

    vector<byte> getJoinTxn(const string& ip, int nonce);

    vector<byte> getSignedJoinTxn(const string& ip, int nonce);

    Response join(const string& ip, int nonce);

    Response join(const string& ip);

    vector<byte> getClaimActiveNodeSpotTxn(int nonce);

    vector<byte> getSignedClaimActiveNodeSpotTxn(int nonce);

    Response claimActiveNodeSpot(int nonce);

    Response claimActiveNodeSpot();

    vector<uint8_t> getDelegateTxn(const std::string& to, long amount, int nonce);

    vector<byte> getSignedDelegateTxn(const string& validator, long amount, int nonce);

    Response delegate(const string& to, long amount, int nonce);

    Response delegate(const string& to, long amount);

    vector<byte> getWithdrawTxn(const string& from, long sharesAmount, int nonce);

    vector<byte> getSignedWithdrawTxn(const string& validator, long sharesAmount, int nonce);

    Response withdraw(const string& from, long sharesAmount, int nonce);

    Response withdraw(const string& from, long sharesAmount);

    vector<byte> getWithdrawPWRTxn(const string& from, long pwrAmount, int nonce);

    vector<byte> getSignedWithdrawPWRTxn(const string& from, long pwrAmount, int nonce);

    Response withdrawPWR(const string& from, long pwrAmount, int nonce);

    Response withdrawPWR(const string& from, long pwrAmount);

    vector<byte> getVmDataTxn(long vmId, const vector<byte>& data, int nonce);

    vector<byte> getSignedVmDataTxn(long vmId, vector<byte> data, int nonce);

    Response removeGuardian(int nonce);

    Response sendVmDataTxn(long vmId, vector<byte> data, int nonce);

    Response sendVmDataTxn(long vmId, vector<byte> data);

    std::vector<std::uint8_t> getClaimVmIdTxn(long vmId, int nonce);

    std::vector<std::uint8_t> getSignedClaimVmIdTxn(long vmId, int nonce);

    Response claimVmId(long vmId, int nonce);

    Response claimVmId(long vmId);

    std::vector<std::uint8_t> getSetGuardianTxn(const std::string& guardianAddress, long expiryDate, int nonce);

    std::vector<uint8_t> getSignedSetGuardianTxn(const std::string& guardianAddress, long expiryDate, int nonce);

    Response setGuardian(const std::string& guardianAddress, long expiryDate, int nonce);

    Response setGuardian(const std::string& guardianAddress, long expiryDate);

    std::vector<byte> getRemoveGuardianTxn(int nonce);

    std::vector<byte> getSignedRemoveGuardianTxn(int nonce);
    
    std::vector<byte> getSignedRemoveGuardianTxn();

    Response removeGuardian(int nonce);

    Response removeGuardian();

    std::vector<byte> getSignedGuardianApprovalTransaction(const vector<vector<uint8_t>>& txn, int nonce);

    Response sendGuardianApprovalTransaction(const vector<vector<uint8_t>>& txn, int nonce);

    std::vector<byte> getPayableVmDataTxn(long vmId, long value, const std::vector<byte>& data, int nonce);

    std::vector<byte> getSignedPayableVmDataTxn(long vmId, long value, const std::vector<byte>& data, int nonce);

    Response sendPayableVmDataTxn(long vmId, long value, const std::vector<byte>& data, int nonce);

    Response sendPayableVmDataTxn(long vmId, long value, const std::vector<byte>& data);

    std::vector<byte> getValidatorRemoveTxn(const std::string& validator, int nonce);

    std::vector<byte> getSignedValidatorRemoveTxn(const std::string& validator, int nonce);

    Response sendValidatorRemoveTxn(const std::string& validator, int nonce);

    Response sendValidatorRemoveTxn(const std::string& validator);

    std::vector<byte> getConduitApprovalTxn(long vmId, const std::vector<std::vector<byte>>& txns, int nonce);

    std::vector<byte> getSignedConduitApprovalTxn(long vmId, const std::vector<std::vector<byte>>& transactions, int nonce);

    Response conduitApprove(long vmId, const std::vector<std::vector<byte>>& transactions, int nonce);

    Response conduitApprove(long vmId, const std::vector<std::vector<byte>>& transactions);

    std::vector<byte> getSetConduitsTxn(long vmId, const std::vector<std::vector<byte>>& conduits, int nonce);

    std::vector<byte> getSignedSetConduitTxn(long vmId, const std::vector<std::vector<byte>>& conduits, int nonce);

    Response setConduits(long vmId, const std::vector<std::vector<byte>>& conduits, int nonce);

    Response setConduits(long vmId, const std::vector<std::vector<byte>>& conduits);

    vector<uint8_t> getSignedMoveStakeTransaction(long sharesAmount, const string& fromValidator, const string& toValidator, int nonce);

    Response moveStake(long sharesAmount, const string& fromValidator, const string& toValidator, int nonce);

    BIGNUM* publicKeyFromPrivate(BIGNUM* privKey);

    EC_POINT* publicPointFromPrivate(BIGNUM* privKey);
};
