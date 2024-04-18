#pragma once

#include <iostream>
#include <string>
#include "Transaction.h"
#include "../json/single_include/nlohmann/json.hpp"

using namespace std;
//using namespace pwr;

using json = nlohmann::json;

class DelegateTxn : public Transaction {
private:
    const string validator;

public:
    static const string type;

    DelegateTxn(int size, long blockNumber, int positionInTheBlock, long fee, const std::string& sender, const std::string& reciever,
        int nonce, const std::string& hash, long timestamp, long value);

    DelegateTxn(const json& blockJson, long blockNumber, long timestamp, int positionInTheBlock);

    // Getters

    /**
     * @return the validator the user is delegating to
     */
    std::string getValidator() const;

};


