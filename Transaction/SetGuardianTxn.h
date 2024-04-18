#pragma once
#include <iostream>
#include <string>
#include "Transaction.h"

//using namespace pwr;

class SetGuardianTxn : public Transaction {
private:
    string guardian;
    long expiryDate;

public:
    static const string type;

    SetGuardianTxn(int size, long blockNumber, int positionInTheBlock, long fee, const std::string& sender, const std::string& reciever,
        int nonce, const std::string& hash, long timestamp, long value, string guardian, long expiryDate);


    SetGuardianTxn(const json& txnObject, long blockNumber, long timestamp, int positionInTheBlock);

    // Getters

    /**
     * @return the validator the user is withdrawing from
     */
    const std::string& getGuardian() const;

    /**
     * @return the amount of PWR the user is withdrawing
     */
    long getExpiryDate() const;

    nlohmann::json toJSON() const;
};

