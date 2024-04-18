#pragma once

#include "../json/single_include/nlohmann/json.hpp"
#include <iostream>
#include <string>
#include "Transaction.h"
#include "TransferTxn.h"
#include "VmDataTxn.h"
#include "DelegateTxn.h"
#include "WithdrawTxn.h"
#include "JoinTxn.h"
#include "ClaimVmIdTxn.h"
#include "PayableVmDataTxn.h"
#include "SetGuardianTxn.h"
#include "GuardianApprovalTxn.h"
#include "ConduitApprovalTxn.h"
#include "RemoveGuardianTxn.h"
#include "ClaimSpotTxn.h"

//using namespace pwr;
using json = nlohmann::json;

Transaction::Transaction(int size, long blockNumber, int positionInTheBlock, long fee,
    const std::string& type, const std::string& sender, const std::string& receiver,
    int nonce, const std::string& hash, long timestamp, long value)
    : size(size),
    blockNumber(blockNumber),
    positionInTheBlock(positionInTheBlock),
    fee(fee),
    type(type),
    sender(sender),
    receiver(receiver),
    nonce(nonce),
    hash(hash),
    timestamp(timestamp),
    value(value) {}

Transaction::Transaction(const json& blockJson, long blockNumber, long timestamp, int positionInTheBlock)
    : size(blockJson.value("size", 0)),
    positionInTheBlock(positionInTheBlock),
    fee(blockJson.value("fee", 0)),
    type(blockJson.value("type", "unknown")),
    sender(blockJson.value("sender", "0x")),
    receiver(blockJson.value("receiver", "0x")),
    nonce(blockJson.value("nonce", 0)),
    hash(blockJson.value("hash", "0x")),
    blockNumber(blockNumber),
    timestamp(timestamp),
    value(blockJson.value("value", 0)) {}

Transaction::Transaction()
    : size(0), blockNumber(0), positionInTheBlock(0), fee(0),
    type(""), sender(""), receiver(""), nonce(0), hash(""), timestamp(0), value(0) {}

Transaction Transaction::fromJSON(const json& json, long blockNumber, long timestamp, int positionInTheBlock) {
    string txnType = json.value("type", "Unknown");

    if (txnType == TransferTxn::type) {
        return TransferTxn(json, blockNumber, timestamp, positionInTheBlock);
    }
    else if (txnType == VmDataTxn::type) {
        return VmDataTxn(json, blockNumber, timestamp, positionInTheBlock);
    }
    else if (txnType == DelegateTxn::type) {
         return DelegateTxn(json, blockNumber, timestamp, positionInTheBlock);
    }
    else if (txnType == WithdrawTxn::type) {
         return WithdrawTxn(json, blockNumber, timestamp, positionInTheBlock);
    }
    else if (txnType == JoinTxn::type) {
        return JoinTxn(json, blockNumber, timestamp, positionInTheBlock);
    }
    else if (txnType == ClaimVmIdTxn::type) {
        return ClaimVmIdTxn(json, blockNumber, timestamp, positionInTheBlock);
    }
    else if (txnType == SetGuardianTxn::type) {
         return SetGuardianTxn(json, blockNumber, timestamp, positionInTheBlock);
    }
    else if (txnType == PayableVmDataTxn::type) {
        return PayableVmDataTxn(json, blockNumber, timestamp, positionInTheBlock);
    }
    else if (txnType == GuardianApprovalTxn::type) {
        return GuardianApprovalTxn(json, blockNumber, timestamp, positionInTheBlock);
    }
    else if (txnType == ConduitApprovalTxn::type) {
        return ConduitApprovalTxn(json, blockNumber, timestamp, positionInTheBlock);
    }
    else if (txnType == RemoveGuardianTxn::type) {
        return RemoveGuardianTxn(json, blockNumber, timestamp, positionInTheBlock);
    }
    else if (txnType == ClaimSpotTxn::type) {
        return ClaimSpotTxn(json, blockNumber, timestamp, positionInTheBlock);
    }
    else {
       return Transaction(json, blockNumber, timestamp, positionInTheBlock);
    }
}
// Getters

/**
 * @return the size of the transaction
 */
int Transaction::getSize() const {
    return size;
}

int Transaction::getBlockNumber() const {
    return blockNumber;
}
/**
 * @return the position of the transaction in the block
 */
int Transaction::getPositionInTheBlock() const {
    return positionInTheBlock;
}

/**
 * @return the fee of the transaction
 */
long Transaction::getFee() const {
    return fee;
}

/**
 * @return the type of the transaction
 */
const std::string& Transaction::getType() const {
    return type;
}

/**
 * @return the address of the sender of the transaction
 */
const std::string& Transaction::getSender() const {
    return sender;
}

/**
 * @return the address of the receiver of the transaction
 */
const std::string& Transaction::getReceiver() const {
    return receiver;
}

/**
 * @return the nonce of the transaction if it is an ECDSA transaction,
 *         or the validation hash if it is an MHBS transaction
 */
const int Transaction::getNonce() const {
    return nonce;
}

/**
 * @return the hash of the transaction
 */
const std::string& Transaction::getHash() const {
    return hash;
}

long Transaction::getTimestamp() const {
    return timestamp;
}


long Transaction::getValue() const {
    return value;
}


//Json function
nlohmann::json Transaction::toJSON()const {
    json jsonObject;

    jsonObject["size"] = size;
    jsonObject["positionInTheBlock"] = positionInTheBlock;
    jsonObject["fee"] = fee;
    jsonObject["type"] = type;
    jsonObject["sender"] = sender;
    jsonObject["receiver"] = receiver;
    jsonObject["nonce"] = nonce;
    jsonObject["hash"] = hash;
    jsonObject["blockNumber"] = getBlockNumber();
    jsonObject["nonce"] = getNonce();
    jsonObject["timestamp"] = getTimestamp();
    jsonObject["value"] = value;

    return jsonObject;
}

//int main() {
//
//}