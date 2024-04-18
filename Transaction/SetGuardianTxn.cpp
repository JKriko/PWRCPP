#include <iostream>
#include <string>
#include "SetGuardianTxn.h"
#include "../json/single_include/nlohmann/json.hpp"

using namespace std;
//using namespace pwr;

using json = nlohmann::json;

SetGuardianTxn::SetGuardianTxn(int size, long blockNumber, int positionInTheBlock, long fee, const std::string& sender, const std::string& reciever,
    int nonce, const std::string& hash, long timestamp, long value, string guardian, long expiryDate)
    : Transaction(size, blockNumber, positionInTheBlock, fee, SetGuardianTxn::type, sender, reciever, nonce, hash, timestamp, value),
    guardian(guardian),
    expiryDate(expiryDate) {}

SetGuardianTxn::SetGuardianTxn(const json& blockObject, long blockNumber, long timestamp, int positionInTheBlock)
    : Transaction(blockObject, blockNumber, timestamp, positionInTheBlock),
    guardian(blockObject.value("guardian", "0x")),
    expiryDate(blockObject.value("expiryDate", 0)) {}

const string SetGuardianTxn::type = "Set guardian";

// Getters

/**
 * @return the validator the user is withdrawing from
 */
const std::string& SetGuardianTxn::getGuardian() const {
    return guardian;
}

/**
 * @return the amount of PWR the user is withdrawing
 */

long SetGuardianTxn::getExpiryDate() const {
    return expiryDate;
}

nlohmann::json SetGuardianTxn::toJSON() const {
    json txn = Transaction::toJSON();
    txn["guardian"] = guardian;
    txn["expiryDate"] = expiryDate;
    return txn;
}



