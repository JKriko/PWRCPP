#include <iostream>
#include "TransactionBuilder.h"

using namespace std;

vector<uint8_t> TransactionBuilder::HexDecode(const std::string& hexString) {
    std::vector<uint8_t> bytes;

    for (size_t i = 0; i < hexString.length(); i += 2) {
        uint8_t byte;
        std::stringstream ss;
        ss << std::hex << hexString.substr(i, 2);
        ss >> byte;
        bytes.push_back(byte);
    }

    return bytes;
}

std::vector<uint8_t> TransactionBuilder::stringToUtf8Bytes(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string utf32 = converter.from_bytes(str);
    std::vector<uint8_t> utf8_bytes(utf32.begin(), utf32.end());
    return utf8_bytes;
}


vector<std::uint8_t> TransactionBuilder::getTransactionBase(std::uint8_t identifier, int nonce, std::uint8_t chainId) {
    std::vector<std::uint8_t> buffer(6);

    buffer[0] = identifier;
    buffer[1] = chainId;
    std::memcpy(&buffer[2], &nonce, sizeof(int));

    return buffer;
}

void TransactionBuilder::assetAddressValidity(const std::string& address) {
    if (address.empty() || (address.length() != 40 && address.length() != 42)) {
        throw std::runtime_error("Invalid address");
    }
}

vector<uint8_t> TransactionBuilder::getTransferPWRTransaction(const std::string& to, long amount, int nonce, uint8_t chainId) {
    assetAddressValidity(to);

    if (amount < 0) {
        throw std::runtime_error("Amount cannot be negative");
    }
    if (nonce < 0) {
        throw std::runtime_error("Nonce cannot be negative");
    }

    vector<uint8_t> TransactionBase = getTransactionBase(static_cast<uint8_t> (0), nonce, chainId);
    vector<uint8_t> result;
    result.reserve(TransactionBase.size() + 8 + 20);

    result.insert(result.end(), TransactionBase.begin(), TransactionBase.end());

    for (int i = sizeof(amount) - 1; i >= 0; --i) {
        uint8_t byteValue = (amount >> (8 * i)) & 0xFF;
        result.push_back(byteValue);
    }

    // Assuming Hex.decode(to) returns the bytes of the address
    std::vector<uint8_t> decodedTo = HexDecode(to);

    result.insert(result.end(), decodedTo.begin(), decodedTo.end());

    return result;
}

vector<uint8_t> TransactionBuilder::getJoinTransaction(const string& ip, int nonce, uint8_t chainId) {
    try {
        vector<uint8_t> txnBase = getTransactionBase(static_cast<uint8_t>(1), nonce, chainId);
        vector<uint8_t> ipBytes = stringToUtf8Bytes(ip);

        vector<uint8_t> buffer;
        buffer.reserve(txnBase.size() + ipBytes.size());

        // Append txnBase
        buffer.insert(buffer.end(), txnBase.begin(), txnBase.end());

        // Append ipBytes
        buffer.insert(buffer.end(), ipBytes.begin(), ipBytes.end());

        return buffer;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}


vector<uint8_t> TransactionBuilder::getClaimActiveNodeSpotTransaction(int nonce, uint8_t chainId) {
    try {
        vector<uint8_t> txnBase = getTransactionBase(static_cast<uint8_t>(2), nonce, chainId);
        return txnBase;
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}


vector<uint8_t> TransactionBuilder::getDelegateTransaction(const std::string& validator, long amount, int nonce, uint8_t chainId) {
    try {
        assetAddressValidity(validator);

        if (amount < 0) {
            throw std::runtime_error("Amount cannot be negative");
        }
        if (nonce < 0) {
            throw std::runtime_error("Nonce cannot be negative");
        }


        vector<uint8_t> TransactionBase = getTransactionBase(static_cast<uint8_t>(3), nonce, chainId);
        vector<uint8_t> result;
        result.reserve(TransactionBase.size() + 28);

        result.insert(result.end(), TransactionBase.begin(), TransactionBase.end());


        // Copy amount to buffer
        for (int i = sizeof(amount) - 1; i >= 0; --i) {
            uint8_t byteValue = (amount >> (8 * i)) & 0xFF;
            result.push_back(byteValue);
        }

        // Assuming Hex.decode(to) returns the bytes of the address
        std::vector<uint8_t> decodedTo = HexDecode(validator);

        result.insert(result.end(), decodedTo.begin(), decodedTo.end());

        return result;
      
    
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

vector<uint8_t> TransactionBuilder::getWithdrawTransaction(const string& validator, long sharesAmount, int nonce, uint8_t chainId) {
    try {
        assetAddressValidity(validator);

        if (sharesAmount < 0) {
            throw std::runtime_error("Amount cannot be negative");
        }
        if (nonce < 0) {
            throw std::runtime_error("Nonce cannot be negative");
        }

        vector<uint8_t> TransactionBase = getTransactionBase(static_cast<uint8_t>(4), nonce, chainId);
        vector<uint8_t> result;
        result.reserve(TransactionBase.size() + 28);

        result.insert(result.end(), TransactionBase.begin(), TransactionBase.end());


        // Copy amount to buffer
        for (int i = sizeof(sharesAmount) - 1; i >= 0; --i) {
            uint8_t byteValue = (sharesAmount >> (8 * i)) & 0xFF;
            result.push_back(byteValue);
        }

        // Assuming Hex.decode(to) returns the bytes of the address
        std::vector<uint8_t> decodedTo = HexDecode(validator);

        result.insert(result.end(), decodedTo.begin(), decodedTo.end());

        return result;
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}


vector<uint8_t> TransactionBuilder::getVmDataTransaction(long vmId, const vector<uint8_t>& data, int nonce, uint8_t chainId) {
    try {
        if (nonce < 0) {
            throw std::runtime_error("Nonce cannot be negative");
        }

        vector<uint8_t> TransactionBase = getTransactionBase(static_cast<uint8_t>(5), nonce, chainId);
        vector<uint8_t> result;
        result.reserve(TransactionBase.size() + 8 + data.size());

        result.insert(result.end(), TransactionBase.begin(), TransactionBase.end());

        // Copy amount to buffer
        for (int i = sizeof(vmId) - 1; i >= 0; --i) {
            uint8_t byteValue = (vmId >> (8 * i)) & 0xFF;
            result.push_back(byteValue);
        }

        result.insert(result.end(), data.begin(), data.end());

        return result;
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
};

vector<std::uint8_t> TransactionBuilder::getClaimVmIdTransaction(long vmId, int nonce, uint8_t chainId) {
    try {

        vector<uint8_t> TransactionBase = getTransactionBase(static_cast<uint8_t>(6), nonce, chainId);
        vector<uint8_t> result;
        result.reserve(TransactionBase.size() + 8);

        result.insert(result.end(), TransactionBase.begin(), TransactionBase.end());

        // Copy amount to buffer
        for (int i = sizeof(vmId) - 1; i >= 0; --i) {
            uint8_t byteValue = (vmId >> (8 * i)) & 0xFF;
            result.push_back(byteValue);
        }

        return result;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

vector<uint8_t> TransactionBuilder::getSetGuardianTransaction(const string& guardian, long expiryDate, int nonce, uint8_t chainId) {
    try {
        assetAddressValidity(guardian);

        if (nonce < 0) {
            throw std::runtime_error("Nonce cannot be negative");
        }
        if (expiryDate < 0) {
            throw std::runtime_error("Expiry date cannot be negative");
        }
        if (expiryDate < std::time(nullptr)) {
            throw std::runtime_error("Expiry date cannot be in the past");
        }

        vector<uint8_t> TransactionBase = getTransactionBase(static_cast<uint8_t>(8), nonce, chainId);
        vector<uint8_t> result;
        result.reserve(TransactionBase.size() + 20 + 8);

        result.insert(result.end(), TransactionBase.begin(), TransactionBase.end());

        // Copy expiryDate to buffer
        for (int i = sizeof(expiryDate) - 1; i >= 0; --i) {
            uint8_t byteValue = (expiryDate >> (8 * i)) & 0xFF;
            result.push_back(byteValue);
        }

        // Decode guardianAddress and copy it to buffer
        vector<uint8_t> decodedAddress = HexDecode(guardian);
        result.insert(result.end(), decodedAddress.begin(), decodedAddress.end());
     

        return result;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

vector<uint8_t> TransactionBuilder::getRemoveGuardianTransaction(int nonce, uint8_t chainId) {
    try {
        std::vector<uint8_t> TransactionBase = getTransactionBase(static_cast<uint8_t>(9), nonce, chainId);
        return TransactionBase;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

vector<uint8_t> TransactionBuilder::getGuardianApprovalTransaction(const vector<vector<uint8_t>>& txn, int nonce, uint8_t chainId) {
    try {
        int totalLength = 0;

        for (const std::vector<uint8_t>& Transaction : txn) {
            totalLength += Transaction.size();
        }
        std::vector<uint8_t> TransactionBase = getTransactionBase(static_cast<uint8_t>(10), nonce, chainId);
        vector<uint8_t> result;
        result.reserve(TransactionBase.size() + (txn.size() * 4) + totalLength);
        
        result.insert(result.end(), TransactionBase.begin(), TransactionBase.end());

        // Add each transaction to the result buffer
        for (const auto& transaction : txn) {
            // Add the length of the transaction to the buffer as a 4-byte integer
            uint32_t transactionLength = static_cast<uint32_t>(transaction.size());
            result.push_back((transactionLength >> 24) & 0xFF);
            result.push_back((transactionLength >> 16) & 0xFF);
            result.push_back((transactionLength >> 8) & 0xFF);
            result.push_back(transactionLength & 0xFF);

            // Add the transaction content to the buffer
            result.insert(result.end(), transaction.begin(), transaction.end());
        }

        return result;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
}

vector<uint8_t> TransactionBuilder::getPayableVmDataTransaction(long vmId, long value, const vector<uint8_t>& data, int nonce, uint8_t chainId) {
    try {
        if (nonce < 0) {
            throw std::runtime_error("Nonce cannot be negative");
        }

        vector<uint8_t> TransactionBase = getTransactionBase(static_cast<uint8_t>(11), nonce, chainId);
        vector<uint8_t> result;
        result.reserve(TransactionBase.size() + 16 + data.size());

        result.insert(result.end(), TransactionBase.begin(), TransactionBase.end());

        // Convert vmId to bytes and copy to buffer
        for (int i = sizeof(vmId) - 1; i >= 0; --i) {
            uint8_t byteValue = (vmId >> (8 * i)) & 0xFF;
            result.push_back(byteValue);
        }

        // Copy data to buffer
        result.insert(result.end(), data.begin(), data.end());

        // Convert value to bytes and copy to buffer
        for (int i = sizeof(value) - 1; i >= 0; --i) {
            uint8_t byteValue = (value >> (8 * i)) & 0xFF;
            result.push_back(byteValue);
        }

        return result;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
}

vector<uint8_t> TransactionBuilder::getValidatorRemoveTransaction(const string& validator, int nonce, uint8_t chainId) {
    try {
        assetAddressValidity(validator);

        // Check if the nonce is negative
        if (nonce < 0) {
            throw std::runtime_error("Nonce cannot be negative");
        }

        vector<uint8_t> TransactionBase = getTransactionBase(static_cast<uint8_t>(7), nonce, chainId);
        vector<uint8_t> result;
        result.reserve(TransactionBase.size() + 20);

        result.insert(result.end(), TransactionBase.begin(), TransactionBase.end());

        // Decode guardianAddress and copy it to buffer
        vector<uint8_t> decodedAddress = HexDecode(validator);
        result.insert(result.end(), decodedAddress.begin(), decodedAddress.end());

        return result;
    }
    catch (const std::exception& e) {
        // Handle the exception accordingly, return an empty byte array
        std::cerr << "Error: " << e.what() << std::endl;
        return std::vector<uint8_t>();
        throw;
    }
}

vector<uint8_t> TransactionBuilder::getConduitApprovalTransaction(long vmId, const vector<vector<uint8_t>>& txns, int nonce, uint8_t chainId) {
    try {
        if (nonce < 0) {
            throw std::runtime_error("Nonce cannot be negative");
        }
        if (txns.size() == 0) {
            throw std::runtime_error("No transactions to approve");
        }

        int totalTxnsLength = 0;
        for (const auto& txn : txns) {
            totalTxnsLength += txn.size();
        }

        std::vector<uint8_t> TransactionBase = getTransactionBase(static_cast<uint8_t>(12), nonce, chainId);
        std::vector<uint8_t> result;
        result.reserve(TransactionBase.size() + 8 + (txns.size() * 4) + totalTxnsLength);

        result.insert(result.end(), TransactionBase.begin(), TransactionBase.end());

        // Convert vmId to bytes and copy to buffer
        for (int i = sizeof(vmId) - 1; i >= 0; --i) {
            uint8_t byteValue = (vmId >> (8 * i)) & 0xFF;
            result.push_back(byteValue);
        }

        // Copy transaction lengths and transactions to buffer
        for (const auto& txn : txns) {
            int txnLength = txn.size();
            for (int i = sizeof(txnLength) - 1; i >= 0; --i) {
                uint8_t byteValue = (txnLength >> (8 * i)) & 0xFF;
                result.push_back(byteValue);
            }
            result.insert(result.end(), txn.begin(), txn.end());
        }

        return result;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
}

vector<uint8_t> TransactionBuilder::getSetConduitsTransaction(long vmId, const vector<vector<uint8_t>>& conduits, int nonce, uint8_t chainId) {
    try {
        if (nonce < 0) {
            throw std::runtime_error("Nonce cannot be negative");
        }
        if (conduits.empty()) {
            throw std::runtime_error("No conduits to set");
        }

        size_t totalConduitLength = 0;
        for (const auto& conduit : conduits) {
            totalConduitLength += conduit.size();
        }

        std::vector<uint8_t> TransactionBase = getTransactionBase(static_cast<uint8_t>(13), nonce, chainId);
        std::vector<uint8_t> result;
        result.reserve(TransactionBase.size() + 8 + (conduits.size() * 4) + totalConduitLength);

        result.insert(result.end(), TransactionBase.begin(), TransactionBase.end());
  

        // Copy vmId to buffer
        for (int i = sizeof(vmId) - 1; i >= 0; --i) {
            uint8_t byteValue = (vmId >> (8 * i)) & 0xFF;
            result.push_back(byteValue);
        }

        // Copy conduits to buffer
        for (const auto& conduit : conduits) {
            int conduitSize = conduit.size();
            for (int i = sizeof(conduitSize) - 1; i >= 0; --i) {
                uint8_t byteValue = (conduitSize >> (8 * i)) & 0xFF;
                result.push_back(byteValue);
            }
            result.insert(result.end(), conduit.begin(), conduit.end());
        }

        return result;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
}

vector<uint8_t> TransactionBuilder::getAddConduitsTransaction(long vmId, const vector<vector<uint8_t>>& conduits, int nonce, uint8_t chainId) {
    try {
        if (nonce < 0) {
            throw std::runtime_error("Nonce cannot be negative");
        }
        if (conduits.size() == 0) {
            throw std::runtime_error("No transactions to approve");
        }

        std::vector<uint8_t> TransactionBase = getTransactionBase(static_cast<uint8_t>(14), nonce, chainId);
        std::vector<uint8_t> result;
        result.reserve(TransactionBase.size() + 8 + (conduits.size() * 20));

        // Copy txnBase to buffer
        result.insert(result.end(), TransactionBase.begin(), TransactionBase.end());

        // Copy vmId to buffer
        for (int i = sizeof(vmId) - 1; i >= 0; --i) {
            uint8_t byteValue = (vmId >> (8 * i)) & 0xFF;
            result.push_back(byteValue);
        }

        // Add each conduit to the result buffer
        for (const auto& conduit : conduits) {
            result.insert(result.end(), conduit.begin(), conduit.end());
        }

        return result;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
}

vector<uint8_t> TransactionBuilder::getRemoveConduitsTxn(long vmId, const vector<vector<uint8_t>>& conduits, int nonce, uint8_t chainId) {
    try {
        if (nonce < 0) {
            throw std::runtime_error("Nonce cannot be negative");
        }
        if (conduits.size() == 0) {
            throw std::runtime_error("No transactions to approve");
        }

        std::vector<uint8_t> TransactionBase = getTransactionBase(static_cast<uint8_t>(15), nonce, chainId);
        std::vector<uint8_t> result;
        result.reserve(TransactionBase.size() + 8 + (conduits.size() * 20));

        // Copy txnBase to buffer
        result.insert(result.end(), TransactionBase.begin(), TransactionBase.end());

        // Copy vmId to buffer
        for (int i = sizeof(vmId) - 1; i >= 0; --i) {
            uint8_t byteValue = (vmId >> (8 * i)) & 0xFF;
            result.push_back(byteValue);
        }

        // Add each conduit to the result buffer
        for (const auto& conduit : conduits) {
            result.insert(result.end(), conduit.begin(), conduit.end());
        }

        return result;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
}

vector<uint8_t> TransactionBuilder::getMoveStakeTransaction(long sharesAmount, const string& fromValidator, const string& toValidator, int nonce, uint8_t chainId) {
    try {
        assetAddressValidity(fromValidator);
        assetAddressValidity(toValidator);

        vector<uint8_t> TransactionBase = getTransactionBase(static_cast<uint8_t>(16), nonce, chainId);
        std::vector<uint8_t> result;
        result.reserve(TransactionBase.size() + 48);

        result.insert(result.end(), TransactionBase.begin(), TransactionBase.end());

        // Copy vmId to buffer
        for (int i = sizeof(sharesAmount) - 1; i >= 0; --i) {
            uint8_t byteValue = (sharesAmount >> (8 * i)) & 0xFF;
            result.push_back(byteValue);
        }

        // Decode guardianAddress and copy it to buffer
        vector<uint8_t> decodedAddress = HexDecode(fromValidator);
        result.insert(result.end(), decodedAddress.begin(), decodedAddress.end());

        // Decode guardianAddress and copy it to buffer
        vector<uint8_t> decodedAddress = HexDecode(toValidator);
        result.insert(result.end(), decodedAddress.begin(), decodedAddress.end());

        return result;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
  
}
