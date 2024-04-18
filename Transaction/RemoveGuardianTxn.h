#pragma once
#include <iostream>
#include <string>
#include "Transaction.h"

//using namespace pwr;

class RemoveGuardianTxn : public Transaction {

public:
    static const string type;

    RemoveGuardianTxn(int size, long blockNumber, int positionInTheBlock, long fee, const std::string& sender, const std::string& reciever,
        int nonce, const std::string& hash, long timestamp, long value);


    RemoveGuardianTxn(const json& txnObject, long blockNumber, long timestamp, int positionInTheBlock);
};


