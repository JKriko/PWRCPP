#include <iostream>
#include <string>
#include "DelegateTxn.h"
#include "../json/single_include/nlohmann/json.hpp"

using namespace std;
///using namespace pwr;

using json = nlohmann::json;

DelegateTxn::DelegateTxn(int size, long blockNumber, int positionInTheBlock, long fee, const std::string& sender, const std::string& reciever,
    int nonce, const std::string& hash, long timestamp, long value)
    : Transaction(size, blockNumber, positionInTheBlock, fee, DelegateTxn::type, sender, reciever, nonce, hash, timestamp, value),
    validator(reciever) {}

DelegateTxn::DelegateTxn(const json& blockJson, long blockNumber, long timestamp, int positionInTheBlock)
    : Transaction(blockJson, blockNumber, timestamp, positionInTheBlock),
    validator(blockJson.value("validator", "0x")) {}

const string DelegateTxn::type = "Delegate";

// Getters

/**
 * @return the validator the user is delegating to
 */
std::string DelegateTxn::getValidator() const {
    return validator;
}



