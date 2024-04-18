#pragma once

#include<iostream>
#include "TxnForGuardianApproval.h"

using namespace std;

TxnForGuardianApproval::TxnForGuardianApproval(bool valid, const string& guardianAddress, const string& errorMessage, Transaction transaction)
    :valid(valid),
    guardianAddress(guardianAddress),
    errorMessage(errorMessage),
    transaction(transaction) {}

TxnForGuardianApproval::TxnForGuardianApproval(bool valid, const string& errorMessage, Transaction transaction)
    :valid(valid),
    errorMessage(errorMessage),
    transaction(transaction) {}

bool TxnForGuardianApproval::getValidation() {
    return valid;
}

string& TxnForGuardianApproval::getGuardianAddress() {
    return guardianAddress;
}

string& TxnForGuardianApproval::getErrorMessage() {
    return errorMessage;
}

Transaction TxnForGuardianApproval::getTransaction() {
    return transaction;
}

//int main() {
//}