#pragma once

#include <iostream>
#include "../json/single_include/nlohmann/json.hpp"
#include "../Transaction/Transaction.h"

using json = nlohmann::json;

using namespace std;

class TxnForGuardianApproval {
private:
    bool valid;
    string guardianAddress;
    string errorMessage;;
    Transaction transaction;

public:
    TxnForGuardianApproval(bool valid, const string& guardianAddress, const string& errorMessage, Transaction transaction);

    TxnForGuardianApproval(bool valid, const string& errorMessage, Transaction transaction);

    bool getValidation();

    string& getGuardianAddress();

    string& getErrorMessage();

    Transaction getTransaction();

};