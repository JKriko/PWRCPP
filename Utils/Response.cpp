#include <iostream>
#include <string>
#include "Response.h"

#pragma once

Response::Response(bool success, std::string txnHash, std::string error)
    : success(success), txnHash(txnHash), error(error) {}

/**
 * @return true if the operation was successful, false otherwise
 */
bool Response::isSuccess() const {
    return success;
}

/**
 * @return transaction hash if the operation was successful
 */
const std::string& Response::getTxnHash() const {
    return txnHash;
}

/**
 * @return error message if the operation was not successful
 */
const std::string& Response::getError() const {
    return error;
}

//int main() {
//    // Example usage:
//    Response response(true, "0x123456789abcdef", "err");
//
//    // Check if the operation was successful
//    if (response.isSuccess()) {
//        std::cout << "Transaction Hash: " << response.getTxnHash() << std::endl;
//    }
//    else {
//        std::cerr << "Error: " << response.getError() << std::endl;
//    }
//
//    return 0;
//}