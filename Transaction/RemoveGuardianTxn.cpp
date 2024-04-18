#include <iostream>
#include <string>
#include "RemoveGuardianTxn.h"
#include "../json/single_include/nlohmann/json.hpp"

using namespace std;
//using namespace pwr;

using json = nlohmann::json;

RemoveGuardianTxn::RemoveGuardianTxn(int size, long blockNumber, int positionInTheBlock, long fee, const std::string& sender, const std::string& reciever,
    int nonce, const std::string& hash, long timestamp, long value)
    : Transaction(size, blockNumber, positionInTheBlock, fee, RemoveGuardianTxn::type, sender, reciever, nonce, hash, timestamp, value) {}

RemoveGuardianTxn::RemoveGuardianTxn(const json& blockObject, long blockNumber, long timestamp, int positionInTheBlock)
    : Transaction(blockObject, blockNumber, timestamp, positionInTheBlock) {}

const string RemoveGuardianTxn::type = "Remove Guardian";


