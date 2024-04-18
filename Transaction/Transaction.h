#pragma once

#include <iostream>
#include <string>
#include "../json/single_include/nlohmann/json.hpp"

//class TransferTxn;
//class VmDataTxn;
//class DelegateTxn;
//class WithdrawTxn;
//class JoinTxn;
//class ClaimVmIdTxn;
//class PayableVmDataTxn;
//class SetGuardianTxn;
//class GuardianApprovalTxn;
//class ConduitApprovalTxn;
//class RemoveGuardianTxn;
//class ClaimSpotTxn;

using json = nlohmann::json;
using namespace std;


class Transaction {
private:
    const int size;
    const long blockNumber;
    const int positionInTheBlock;
    const long fee;
    const string type;
    const string sender;
    const string receiver;
    const int nonce;
    const string hash;
    const long timestamp;
    const long value;

public:
    Transaction();

    Transaction(int size, long blockNumber, int positionInTheBlock, long fee,
        const std::string& type, const std::string& sender, const std::string& receiver,
        int nonce, const std::string& hash, long timestamp, long value);

    Transaction(const json& jsonObject, long blockNumber, long timestamp, int positionInTheBlock);

    static Transaction fromJSON(const json& json, long blockNumber, long timestamp, int positionInTheBlock);

    // Getters

    /**
    * @return the size of the transaction
    */
    int getSize() const;

    /**
     * @return the position of the transaction in the block
     */

    int getBlockNumber() const;

    int getPositionInTheBlock() const;

    /**
     * @return the fee of the transaction
     */

    long getFee() const;

    /**
     * @return the type of the transaction
     */

    const std::string& getType() const;

    /**
     * @return the address of the sender of the transaction
     */

    const std::string& getSender() const;

    /**
     * @return the address of the receiver of the transaction
     */

    const std::string& getReceiver() const;

    /**
     * @return the nonce of the transaction if it is an ECDSA transaction,
     *         or the validation hash if it is an MHBS transaction
     */

    const int getNonce() const;

    /**
     * @return the hash of the transaction
     */

    const std::string& getHash() const;

    long getTimestamp() const;

    // Abstract Methods
    long getValue() const;

    nlohmann::json toJSON() const;
};



