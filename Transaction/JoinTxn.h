#pragma once

#include <iostream>
#include <string>
#include "Transaction.h"
#include "../json/single_include/nlohmann/json.hpp"

using namespace std;
//using namespace pwr;

using json = nlohmann::json;

class JoinTxn : public Transaction {
private:
    const string validator;

public:
    static const string type;

    JoinTxn(int size, long blockNumber, int positionInTheBlock, long fee, const std::string& sender, const std::string& reciever,
        int nonce, const std::string& hash, long timestamp, long value);

    JoinTxn(const json& blockJson, long blockNumber, long timestamp, int positionInTheBlock);

    // Getter

    /**
     * @return the validator the user is joining
     */
    std::string getValidator() const;

    nlohmann::json toJSON() const;
};

