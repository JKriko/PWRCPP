#include <iostream>
#include <string>
#include "GuardianApprovalTxn.h"
#include "../json/single_include/nlohmann/json.hpp"

using namespace std;
//using namespace pwr;

using json = nlohmann::json;

GuardianApprovalTxn::GuardianApprovalTxn(int size, long blockNumber, int positionInTheBlock, long fee, const std::string& sender, const std::string& reciever,
    int nonce, const std::string& hash, long timestamp, long value)
    : Transaction(size, blockNumber, positionInTheBlock, fee, GuardianApprovalTxn::type, sender, reciever, nonce, hash, timestamp, value) {}

GuardianApprovalTxn::GuardianApprovalTxn(const json& blockObject, long blockNumber, long timestamp, int positionInTheBlock)
    : Transaction(blockObject, blockNumber, timestamp, positionInTheBlock) {
    json transactions(blockObject.value("transactions", json::array()));
    if (!transactions.empty()) {
        for (const auto& transaction : transactions) {
            this->transactions.push_back(transaction.get<std::string>());
        }
    }
}

const string GuardianApprovalTxn::type = "Guardian Approval";

// Getters



const vector<string> GuardianApprovalTxn::getTransactions() const {
    return transactions;
}

nlohmann::json GuardianApprovalTxn::toJSON() const {
    json txn = Transaction::toJSON();
    json transactions = json::array();
    for (string transaction : this->transactions) {
        transactions.push_back(transaction);
    }

    txn["transactions"] = transactions;

    return txn;
}

