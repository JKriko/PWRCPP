#include <iostream>
#include <string>
#include "PayableVmDataTxn.h"
#include "../json/single_include/nlohmann/json.hpp"

using namespace std;
//using namespace pwr;

using json = nlohmann::json;

PayableVmDataTxn::PayableVmDataTxn(int size, long blockNumber, int positionInTheBlock, long fee, const std::string& sender, const std::string& reciever,
    int nonce, const std::string& hash, long timestamp, long value, long vmId, const std::string& data)
    : Transaction(size, blockNumber, positionInTheBlock, fee, PayableVmDataTxn::type, sender, reciever, nonce, hash, timestamp, value),
    vmId(vmId),
    data(data) {}

PayableVmDataTxn::PayableVmDataTxn(const json& txnObject, long blockNumber, long timestamp, int positionInTheBlock)
    : Transaction(txnObject, blockNumber, timestamp, positionInTheBlock),
    vmId(txnObject["vmId"].get<long>()),
    data(txnObject["data"].get<string>()) {}

const string PayableVmDataTxn::type = "Payable VM Data";

// Getters

/**
 * @return the id of the VM that this transaction is for
 */
long PayableVmDataTxn::getVmId() const {
    return vmId;
}

/**
 * @return the data of the transaction
 */
const std::string& PayableVmDataTxn::getData() const {
    return data;
}

nlohmann::json PayableVmDataTxn::toJSON() const {
    json txn = Transaction::toJSON();
    txn["vmId"] = vmId;
    txn["data"] = data;
    return txn;
}

