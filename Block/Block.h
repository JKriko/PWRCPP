#include <iostream>
#include <vector>
#include <string>
#include "../json/single_include/nlohmann/json.hpp"
//#include "../Transaction/Transaction.h"
#include "../Transaction/Transaction.h"
//#include "../Protocol/PWRJ.h"

using json = nlohmann::json;

class Block {
private:
    int transactionCount;
    int size;
    long number;
    long reward;
    long timestamp;
    std::string hash;
    std::string submitter;
    bool success;
    std::vector<Transaction> transactions;

public:
    Block(const nlohmann::json& blockJson);


    // Getters

    /**
     * @return the number of transactions in the block
     */
    int getTransactionCount() const;

    /**
     * @return the byte size of the block
     */
    int getSize() const;

    /**
     * @return the number of the block
     */
    long getNumber() const;

    /**
     * @return the reward of the block
     */
    long getReward() const;

    /**
     * @return the timestamp of the block
     */
    long getTimestamp() const;

    /**
     * @return the hash of the block
     */
    const std::string& getHash() const;

    /**
     * @return the address of the submitter of the block
     */
    const std::string& getSubmitter() const;
    /**
     * @return whether the block was processed without issues
     */
    bool isSuccess() const;

    /**
     * @return the transactions in the block
     */
    const std::vector<Transaction>& getTransactions() const;
};
