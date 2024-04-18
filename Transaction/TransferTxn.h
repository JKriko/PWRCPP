#pragma once

#include <iostream>
#include <string>
#include "Transaction.h"

//using namespace pwr;

class TransferTxn : public Transaction {
public:
    static const string type;

    TransferTxn(int size, long blockNumber, int positionInTheBlock, long fee, const std::string& sender, const std::string& reciever,
        int nonce, const std::string& hash, long timestamp, long value);

    TransferTxn(const json& blockJson, long blockNumber, long timestamp, int positionInTheBlock);

    //nlohmann::json toJSON() const;
};


