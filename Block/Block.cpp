#include <iostream>
#include <vector>
#include <string>
#include "Block.h"





using namespace std;
using json = nlohmann::json;

Block::Block(const json& blockJson)
    : transactionCount(blockJson.value("transactionCount", 0)),
    size(blockJson.value("blockSize", 0)),
    number(blockJson.value("blockNumber", 0)),
    reward(blockJson.value("blockReward", 0)),
    timestamp(blockJson.value("timestamp", 0)),
    hash(blockJson.value("blockHash", "")),
    submitter(blockJson.value("blockSubmitter", "")),
    success(blockJson.value("success", false)) {

    json txns = json::array();
    txns = blockJson.value("transactions", json::array());
    transactions.reserve(txns.size());

    int i = 0;

    for (const auto& txnObjects : txns) {
        json txnObject = txnObjects;
        Transaction txn = Transaction::fromJSON(txnObject, number, timestamp, i);
        transactions.push_back(txn);
        i++;
    }
}


// Getters

/**
 * @return the number of transactions in the block
 */
int Block::getTransactionCount() const {
    return transactionCount;
}

/**
 * @return the byte size of the block
 */
int Block::getSize() const {
    return size;
}

/**
 * @return the number of the block
 */
long Block::getNumber() const {
    return number;
}

/**
 * @return the reward of the block
 */
long Block::getReward() const {
    return reward;
}

/**
 * @return the timestamp of the block
 */
long Block::getTimestamp() const {
    return timestamp;
}

/**
 * @return the hash of the block
 */
const std::string& Block::getHash() const {
    return hash;
}

/**
 * @return the address of the submitter of the block
 */
const std::string& Block::getSubmitter() const {
    return submitter;
}

/**
 * @return whether the block was processed without issues
 */
bool Block::isSuccess() const {
    return success;
}

/**
 * @return the transactions in the block
 */
const std::vector<Transaction>& Block::getTransactions() const {
    return transactions;
}

 //int main() {
 //    // Example usage:
 //    // Populate blockJson with your JSON data
 //    json blockJson = R"(
 //       {
 //           "transactionCount": 3,
 //           "blockSize": 256,
 //           "blockNumber": 123456,
 //           "blockReward": 10,
 //           "timestamp": 1647860712,
 //           "blockHash": "0x123456789abcdef",
 //           "blockSubmitter": "Alice",
 //           "success": true,
 //           "transactions": [
 //               {
 //                   "transactionId": "tx123",
 //                   "from": "Alice",
 //                   "to": "Bob",
 //                   "amount": 5
 //               },
 //               {
 //                   "transactionId": "tx456",
 //                   "from": "Bob",
 //                   "to": "Charlie",
 //                   "amount": 10
 //               },
 //               {
 //                   "transactionId": "tx789",
 //                   "from": "Charlie",
 //                   "to": "Alice",
 //                   "amount": 7
 //               }
 //           ]
 //       }
 //   )"_json;

 //    Block block(blockJson);

 //    std::cout << "Transaction Count: " << block.getTransactionCount() << std::endl;
 //    std::cout << "Size: " << block.getSize() << std::endl;
 //    std::cout << "Number: " << block.getNumber() << std::endl;
 //    std::cout << "Reward: " << block.getReward() << std::endl;
 //    std::cout << "Timestamp: " << block.getTimestamp() << std::endl;
 //    std::cout << "Hash: " << block.getHash() << std::endl;
 //    std::cout << "Submitter: " << block.getSubmitter() << std::endl;
 //    std::cout << "Success: " << block.isSuccess() << std::endl;

 //    const auto& transactions = block.getTransactions();
 //    for (const auto& txn : transactions) {
 //        // Handle transactions as needed
 //        
 //    }

 //    return 0;
 //}


//int main() {
//	cout << "Hello World" << endl;
//}