#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Transaction.h"

//using namespace pwr;

class GuardianApprovalTxn : public Transaction {
private:
    std::vector<string> transactions;
public:
    static const string type;

    GuardianApprovalTxn(int size, long blockNumber, int positionInTheBlock, long fee, const std::string& sender, const std::string& reciever,
        int nonce, const std::string& hash, long timestamp, long value);


    GuardianApprovalTxn(const json& txnObject, long blockNumber, long timestamp, int positionInTheBlock);

    // Getters

    const vector<string> getTransactions() const;


    nlohmann::json toJSON() const;
};


