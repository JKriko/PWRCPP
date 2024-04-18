#include <iostream>
#include <string>
#include "WithdrawTxn.h"
#include "../json/single_include/nlohmann/json.hpp"

using namespace std;
//using namespace pwr;

using json = nlohmann::json;

WithdrawTxn::WithdrawTxn(int size, long blockNumber, int positionInTheBlock, long fee, const std::string& sender, const std::string& reciever,
    int nonce, const std::string& hash, long timestamp, long value, long shares)
    : Transaction(size, blockNumber, positionInTheBlock, fee, WithdrawTxn::type, sender, reciever, nonce, hash, timestamp, value),
    validator(reciever),
    shares(shares) {}

WithdrawTxn::WithdrawTxn(const json& blockObject, long blockNumber, long timestamp, int positionInTheBlock)
    : Transaction(blockObject, blockNumber, timestamp, positionInTheBlock),
    validator(blockObject.value("validator", "0x")),
    shares(blockObject.value("shares", 0)) {}

const string WithdrawTxn::type = "Withdraw";

// Getters

/**
 * @return the validator the user is withdrawing from
 */
const std::string& WithdrawTxn::getValidator() const {
    return validator;
}

/**
 * @return the amount of PWR the user is withdrawing
 */

long WithdrawTxn::getShares() const {
    return shares;
}

nlohmann::json WithdrawTxn::toJSON() const {
    json txn = Transaction::toJSON();
    txn["validator"] = validator;
    txn["shares"] = shares;
    return txn;
}



