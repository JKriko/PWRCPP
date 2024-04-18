#include <iostream>
#include <string>
#include "Transaction.h"
#include "ConduitApprovalTxn.h"
#include "../json/single_include/nlohmann/json.hpp"

using namespace std;
//using namespace pwr;

using json = nlohmann::json;

ConduitApprovalTxn::ConduitApprovalTxn(const json& blockObject, long blockNumber, long timestamp, int positionInTheBlock)
    : Transaction(blockObject, blockNumber, timestamp, positionInTheBlock),
    vmId(blockObject.value("vmId", 0)) {
    json transactions(blockObject.value("transactions", json::array()));
    if (!transactions.empty()) {
        for (const auto& transaction : transactions) {
            this->transactions.push_back(transaction.get<std::string>());
        }
    }
}

const string ConduitApprovalTxn::type = "Conduit Approval";