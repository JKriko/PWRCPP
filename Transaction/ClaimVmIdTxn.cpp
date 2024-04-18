#include <iostream>
#include <string>
#include "ClaimVmIdTxn.h"
#include "../json/single_include/nlohmann/json.hpp"

using namespace std;
//using namespace pwr;

using json = nlohmann::json;

ClaimVmIdTxn::ClaimVmIdTxn(int size, long blockNumber, int positionInTheBlock, long fee, const std::string& sender, const std::string& reciever,
    int nonce, const std::string& hash, long timestamp, long value, long vmId)
    : Transaction(size, blockNumber, positionInTheBlock, fee, ClaimVmIdTxn::type, sender, reciever, nonce, hash, timestamp, value),
    vmId(vmId) {}


ClaimVmIdTxn::ClaimVmIdTxn(const json& blockJson, long blockNumber, long timestamp, int positionInTheBlock)
    : Transaction(blockJson, blockNumber, timestamp, positionInTheBlock),
    vmId(blockJson.value("vmId", 0)) {}

const string ClaimVmIdTxn::type = "Claim VM ID";

// Getters

/**
 * @return the id of the VM that this transaction is for
 */
long ClaimVmIdTxn::getVmId() const {
    return vmId;
}


nlohmann::json ClaimVmIdTxn::toJSON() const {
    json txn = Transaction::toJSON();
    txn["vmId"] = vmId;
    return txn;
}

