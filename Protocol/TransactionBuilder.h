#pragma once

#include<iostream>
#include <vector>
#include <string>
#include <cstddef> // Include for std::byte
#include <codecvt>
#include <sstream>
#include <iomanip>


using namespace std;

class TransactionBuilder {
private:
    static vector<std::uint8_t> getTransactionBase(std::uint8_t identifier, int nonce, std::uint8_t chainId);

    static void assetAddressValidity(const std::string& address);

public:
    static vector<uint8_t> getTransferPWRTransaction(const std::string& to, long amount, int nonce, uint8_t chainId);

    static vector<uint8_t> HexDecode(const std::string& hexString);

    static vector<uint8_t> getJoinTransaction(const string& ip, int nonce, uint8_t chainId);

    static vector<uint8_t> stringToUtf8Bytes(const std::string& str);

    static vector<uint8_t> getClaimActiveNodeSpotTransaction(int nonce, uint8_t chainId);

    static vector<uint8_t> getDelegateTransaction(const std::string& validator, long amount, int nonce, uint8_t chainId);

    static vector<uint8_t> getWithdrawTransaction(const string& validator, long sharesAmount, int nonce, uint8_t chainId);

    static vector<uint8_t> getVmDataTransaction(long vmId, const vector<uint8_t>& data, int nonce, uint8_t chainId);

    static vector<std::uint8_t> getClaimVmIdTransaction(long vmId, int nonce, uint8_t chainId);

    static vector<uint8_t> getSetGuardianTransaction(const string& guardian, long expiryDate, int nonce, uint8_t chainId);

    static vector<uint8_t> getRemoveGuardianTransaction(int nonce, uint8_t chainId);

    static vector<uint8_t> getGuardianApprovalTransaction(const vector<vector<uint8_t>>& txn, int nonce, uint8_t chainId);

    static vector<uint8_t> getPayableVmDataTransaction(long vmId, long value, const vector<uint8_t>& data, int nonce, uint8_t chainId);

    static vector<uint8_t> getValidatorRemoveTransaction(const string& validator, int nonce, uint8_t chainId);

    static vector<uint8_t> getConduitApprovalTransaction(long vmId, const vector<vector<uint8_t>>& txns, int nonce, uint8_t chainId);

    static vector<uint8_t> getSetConduitsTransaction(long vmId, const vector<vector<uint8_t>>& conduits, int nonce, uint8_t chainId);

    static vector<uint8_t> getAddConduitsTransaction(long vmId, const vector<vector<uint8_t>>& conduits, int nonce, uint8_t chainId);

    static vector<uint8_t> getRemoveConduitsTxn(long vmId, const vector<vector<uint8_t>>& conduits, int nonce, uint8_t chainId);

    static vector<uint8_t> getMoveStakeTransaction(long sharesAmount, const string& fromValidator, const string& toValidator, int nonce, uint8_t chainId);
};
