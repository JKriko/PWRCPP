#include <iostream>
#include <string>
#include "Transaction.h"
#include "ClaimSpotTxn.h"
#include "../json/single_include/nlohmann/json.hpp"

using namespace std;
//using namespace pwr;

using json = nlohmann::json;


ClaimSpotTxn::ClaimSpotTxn(int size, long blockNumber, int positionInTheBlock, long fee, const std::string& sender, const std::string& reciever,
    int nonce, const std::string& hash, long timestamp, long value, string validator)
    : Transaction(size, blockNumber, positionInTheBlock, fee, ClaimSpotTxn::type, sender, reciever, nonce, hash, timestamp, value),
    validator(validator) {}


ClaimSpotTxn::ClaimSpotTxn(const json& blockJson, long blockNumber, long timestamp, int positionInTheBlock)
    : Transaction(blockJson, blockNumber, timestamp, positionInTheBlock),
    validator(Transaction::getSender()) {}


const string ClaimSpotTxn::type = "Validator Claim Spot";

// Getters

/**
 * @return the id of the VM that this transaction is for
 */
const string& ClaimSpotTxn::getValidator() const {
    return validator;
}


nlohmann::json ClaimSpotTxn::toJSON() const {
    json txn = Transaction::toJSON();
    txn["validator"] = validator;
    return txn;
}