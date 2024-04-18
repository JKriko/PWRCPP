#pragma once

#include <iostream>
#include <string>
#include "Transaction.h"
#include "../json/single_include/nlohmann/json.hpp"

using namespace std;
//using namespace pwr;

using json = nlohmann::json;

class ClaimSpotTxn : public Transaction {
private:
    const string validator;

public:
    static const string type;

    ClaimSpotTxn(int size, long blockNumber, int positionInTheBlock, long fee, const std::string& sender, const std::string& reciver,
        int nonce, const std::string& hash, long timestamp, long value, string validator);

    ClaimSpotTxn(const json& txnObject, long blockNumber, long timestamp, int positionInTheBlock);

    // Getters

    /**
     * @return the id of the VM that this transaction is for
     */
    const string& getValidator() const;


    nlohmann::json toJSON() const;
};

