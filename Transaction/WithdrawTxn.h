#pragma once

#include <iostream>
#include <string>
#include "Transaction.h"


//using namespace pwr;

class WithdrawTxn : public Transaction {
private:
    const string validator;
    const long shares;

public:
    static const string type;

    WithdrawTxn(int size, long blockNumber, int positionInTheBlock, long fee, const std::string& sender, const std::string& toreciever,
        int nonce, const std::string& hash, long timestamp, long value, long shares);


    WithdrawTxn(const json& txnObject, long blockNumber, long timestamp, int positionInTheBlock);

    // Getters

    /**
     * @return the validator the user is withdrawing from
     */
    const std::string& getValidator() const;

    /**
     * @return the amount of PWR the user is withdrawing
     */
    long getShares() const;

    nlohmann::json toJSON() const;
};

