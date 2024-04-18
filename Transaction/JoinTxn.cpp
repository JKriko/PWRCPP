#include <iostream>
#include <string>
#include "JoinTxn.h"
#include "../json/single_include/nlohmann/json.hpp"

using namespace std;
//using namespace pwr;

using json = nlohmann::json;

JoinTxn::JoinTxn(int size, long blockNumber, int positionInTheBlock, long fee, const std::string& sender, const std::string& to,
    int nonce, const std::string& hash, long timestamp, long value)
    : Transaction(size, blockNumber, positionInTheBlock, fee, JoinTxn::type, sender, to, nonce, hash, timestamp, value),
    validator(sender) {}

JoinTxn::JoinTxn(const json& blockJson, long blockNumber, long timestamp, int positionInTheBlock)
    : Transaction(blockJson, blockNumber, timestamp, positionInTheBlock),
    validator(blockJson.value("sender", "0x")) {}

const string JoinTxn::type = "Validator Join";

// Getter

/**
 * @return the validator the user is joining
 */
std::string JoinTxn::getValidator() const {
    return validator;
}

nlohmann::json JoinTxn::toJSON() const {
    json txn = Transaction::toJSON();
    txn["validator"] = validator;
    return txn;
}

