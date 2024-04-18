#pragma once

#include <iostream>
#include <string>
#include "Transaction.h"
#include "../json/single_include/nlohmann/json.hpp"

using namespace std;
//using namespace pwr;

using json = nlohmann::json;

class ClaimVmIdTxn : public Transaction {
private:
    const long vmId;

public:
    static const string type;

    ClaimVmIdTxn(int size, long blockNumber, int positionInTheBlock, long fee, const std::string& sender, const std::string& reciver,
        int nonce, const std::string& hash, long timestamp, long value, long vmId);

    ClaimVmIdTxn(const json& txnObject, long blockNumber, long timestamp, int positionInTheBlock);

    // Getters

    /**
     * @return the id of the VM that this transaction is for
     */
    long getVmId() const;


    nlohmann::json toJSON() const;
};



