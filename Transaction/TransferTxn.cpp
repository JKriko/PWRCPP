#include <iostream>
#include <string>
#include "TransferTxn.h"
#include "../json/single_include/nlohmann/json.hpp"

using namespace std;
//using namespace pwr;

using json = nlohmann::json;

TransferTxn::TransferTxn(int size, long blockNumber, int positionInTheBlock, long fee, const std::string& sender, const std::string& reciever,
    int nonce, const std::string& hash, long timestamp, long value)
    : Transaction(size, blockNumber, positionInTheBlock, fee, TransferTxn::type, sender, reciever, nonce, hash, timestamp, value) {}

TransferTxn::TransferTxn(const json& blockJson, long blockNumber, long timestamp, int positionInTheBlock)
    : Transaction(blockJson, blockNumber, timestamp, positionInTheBlock) {}

const string TransferTxn::type = "Transfer";
// Getter

//nlohmann::json TransferTxn::toJSON() const{
//    json txn = Transaction::toJSON();
//    return txn;
//}



