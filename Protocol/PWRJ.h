#pragma once

#include <iostream>
#include <string>
#include <stdexcept> // for std::runtime_error
#include <cstddef> // Include for std::byte
#include "../json/single_include/nlohmann/json.hpp"
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "../cpp-httplib/httplib.h"
#include "../Transaction/VmDataTxn.h"
#include "../Transaction/Transaction.h"
#include "../ResponseModels/TxnForGuardianApproval.h"
#include "../Validator/Validator.h"
#include "../Utils/Response.h"
#include "../cryptopp/keccak.h"
//#include "boost/multiprecision/cpp_bin_float.hpp"
#include <boost/multiprecision/cpp_dec_float.hpp>
#include "../Block/Block.h"
#include "../Utils/Hash.h"



//class Validator;
using json = nlohmann::json;
using namespace std;

//namespace mp = boost::multiprecision;
using BigDecimal = mp::cpp_dec_float_50;
using BigInteger = mp::cpp_int;



class PWRJ {
private:
	static string rpcNodeUrl;
	static byte chainId;
	static long feePerByte;

public:
	PWRJ(const std::string& rpcNodeUrl);

	static std::string byteArrayToHexString(const vector<byte>& byteArray);

	static json httpPost(const std::string& url, const json& body);

	 static json httpGet(const std::string& url);

	 static std::string getRpcNodeUrl();

	 static byte getChainId();

	 static long getFeePerByte();

	 static string getVmIdAddress(long vmId);

	 static short getBlockchainVersion();

	 static int getNonceOfAddress(std::string address);

	 static int64_t getBalanceOfAddress(std::string address);

	 static std::string getGuardianOfAddress(std::string address);

	 static long getBlocksCount();
	
	 static long getLatestBlockNumber();
	 
	 static Block getBlockByNumber(long blockNumber);
	
	 static vector<VmDataTxn> getVMDataTransactions(long startingBlock, long endingBlock, long vmId);
	
	 static vector<VmDataTxn> getVMDataTransactionsFilterByBytePrefix(long startingBlock, long endingBlock, long vmId, vector<byte> prefix);

	static TxnForGuardianApproval isTransactionValidForGuardianApproval(const std::string& txn);
	
	static TxnForGuardianApproval isTransactionValidForGuardianApproval(const std::vector<byte>& txn);

	 static int64_t getActiveVotingPower();
	
	 static int getTotalValidatorsCount();
	 
	 static int getStandbyValidatorsCount();
	
	 static int getActiveValidatorsCount();
	
	 static int getTotalDelegatorsCount();

	 static vector<Validator> getAllValidators();

	 static vector<Validator> getStandbyValidators();
	
	 static vector<Validator> getActiveValidators();

	 static vector<Validator> getDelegatees(const std::string& address);
	 
	 static Validator getValidator(const std::string& validatorAddress);
	
	 static int64_t getDelegatedPWR(const std::string& delegatorAddress, const std::string& validatorAddress);
	
	 static BigDecimal getShareValue(const std::string& validator);
	
	 static std::string getOwnerOfVm(long vmId);
	 
	 static void updateFeePerByte();

	 static Response broadcastTxn(const vector<byte>& txn);

	template<typename T>
	static T getOrDefault(const json& json, std::string key, const T& defaultValue);
};

