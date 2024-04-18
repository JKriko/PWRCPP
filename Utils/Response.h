#include <iostream>
#include <string>

#pragma once

class Response {
private:
    bool success;
    std::string txnHash;
    std::string error;

public:
    Response(bool success, std::string txnHash, std::string error);

    /**
     * @return true if the operation was successful, false otherwise
     */
    bool isSuccess() const;

    /**
     * @return transaction hash if the operation was successful
     */
    const std::string& getTxnHash() const;

    /**
     * @return error message if the operation was not successful
     */
    const std::string& getError() const;
};

