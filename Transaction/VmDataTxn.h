#pragma once

#include <iostream>
#include <string>
#include "Transaction.h"
#include "../json/single_include/nlohmann/json.hpp"

using namespace std;
//using namespace pwr;

using json = nlohmann::json;

class VmDataTxn : public Transaction {
private:
    const long vmId;
    const string data;

public:
    static const string type;

    VmDataTxn(int size, long blockNumber, int positionInTheBlock, long fee, const std::string& sender, const std::string& reciever,
        int nonce, const std::string& hash, long timestamp, long value, long vmId, const std::string& data);


    VmDataTxn(const json& txnObject, long blockNumber, long timestamp, int positionInTheBlock);

    VmDataTxn() = default;

    // Getters

    /**
     * @return the id of the VM that this transaction is for
     */
    long getVmId() const;

    /**
     * @return the data of the transaction
     */
    const std::string& getData() const;

    nlohmann::json toJSON() const;
};



