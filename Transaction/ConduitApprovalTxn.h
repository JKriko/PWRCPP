#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Transaction.h"

//using namespace pwr;

class ConduitApprovalTxn : public Transaction {
private:
    std::vector<string> transactions;
    long vmId;
public:
    static const string type;

    ConduitApprovalTxn(const json& txnObject, long blockNumber, long timestamp, int positionInTheBlock);
};


