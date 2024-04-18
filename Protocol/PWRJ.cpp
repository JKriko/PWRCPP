
#include <iostream>
#include "PWRJ.h"

//class Validator;

using json = nlohmann::json;
using namespace std;

// Define static member variables
string PWRJ::rpcNodeUrl;
long PWRJ::feePerByte = 0;
byte PWRJ::chainId;

// Constructor definition
PWRJ::PWRJ(const std::string& rpcNodeUrl){
	PWRJ::rpcNodeUrl = rpcNodeUrl;

	try {
		json object = httpGet(rpcNodeUrl + "/chainId/");
		chainId = static_cast<byte>(object["chainId"].get<int>());
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Failed to get chain ID from the RPC node: " + std::string(e.what()));
	}

	try {
		json object = httpGet(rpcNodeUrl + "/feePerByte/");
		feePerByte = object["feePerByte"].get<long>();
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Failed to get fee per byte from the RPC node: " + std::string(e.what()));
	}
}



std::string PWRJ::byteArrayToHexString(const vector<byte>& byteArray) {
	std::ostringstream oss;
	oss << std::hex << std::uppercase << std::setfill('0');
	for (unsigned char byte : byteArray) {
		oss << std::setw(2) << static_cast<int>(byte);
	}
	return oss.str();
}

json PWRJ::httpGet(const std::string& url) {
	try {
		// Extract the hostname and path from the URL
		std::string hostname;
		std::string path;

		size_t pos = url.find("://");
		if (pos != std::string::npos) {
			pos += 3; // Skip past "://"
			size_t end = url.find('/', pos);
			if (end != std::string::npos) {
				hostname = url.substr(pos, end - pos);
				path = url.substr(end);
			}
			else {
				hostname = url.substr(pos);
				path = "/";
			}
		}
		else {
			// URL format is invalid
			throw std::runtime_error("Invalid URL format");
		}

		// Set timeouts
		int connectionTimeout = 5 * 1000; // 5 seconds
		int socketTimeout = 5 * 1000; // 5 seconds

		// Create SSLClient with just the hostname
		httplib::SSLClient client(hostname.c_str());

		// Set timeouts directly on the client instance
		client.set_connection_timeout(connectionTimeout);
		client.set_read_timeout(socketTimeout);

		// Make the request using the extracted path
		auto res = client.Get(path.c_str());

		if (res && res->status == 200) {
			// Successful response, return JSON object
			return json::parse(res->body);
		}
		else if (res && res->status == 400) {
			// Handle HTTP 400 error
			std::string message = res->body;
			throw std::runtime_error("Failed with HTTP error 400 and message: " + message);
		}
		else {
			// Handle other HTTP errors
			throw std::runtime_error("Failed with HTTP error code: " + std::to_string(res ? res->status : -1));
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred: " << e.what() << std::endl;
		// Return an empty JSON object to indicate failure
		return json();
	}
}

json PWRJ::httpPost(const std::string& url, const json& body) {
	try {
		// Extract the hostname and path from the URL
		std::string hostname;
		std::string path;

		size_t pos = url.find("://");
		if (pos != std::string::npos) {
			pos += 3; // Skip past "://"
			size_t end = url.find('/', pos);
			if (end != std::string::npos) {
				hostname = url.substr(pos, end - pos);
				path = url.substr(end);
			}
			else {
				hostname = url.substr(pos);
				path = "/";
			}
		}
		else {
			// URL format is invalid
			throw std::runtime_error("Invalid URL format");
		}

		//string fullHostUrl = "https://" + hostname;

		// Set timeouts
		int connectionTimeout = 5 * 1000; // 5 seconds
		int socketTimeout = 5 * 1000; // 5 seconds

		// Create SSLClient with just the hostname
		httplib::SSLClient client(hostname.c_str());

		// Set timeouts directly on the client instance
		client.set_connection_timeout(connectionTimeout);
		client.set_read_timeout(socketTimeout);

		// Create headers
		httplib::Headers headers = {
			{"Accept", "application/json"},
			{"Content-type", "application/json"}
		};

		// Send POST request with JSON body
		auto res = client.Post(path.c_str(), headers, body.dump(), "application/json");

		if (res && res->status == 200) {
			return json::parse(res->body);
		}
		else if (res && res->status == 400) {
			json error_json = json::parse(res->body);
			throw runtime_error("Failed with HTTP error 400 and message: " + error_json["message"].get<std::string>());
		}
		else {
			throw runtime_error("Failed with HTTP error code: " + to_string(res ? res->status : -1));
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred: " << e.what() << std::endl;
		// Return an empty JSON object to indicate failure
		return json();
	}
}

string PWRJ::getVmIdAddress(long vmId) {
	std::string hexAddress = vmId >= 0 ? "1" : "0";
	if (vmId < 0) vmId = -vmId;
	std::string vmIdString = std::to_string(vmId);

	for (int i = 0; i < 39 - vmIdString.length(); i++) {
		hexAddress += "0";
	}

	hexAddress += vmIdString;

	return "0x" + hexAddress;
}

std::string PWRJ::getRpcNodeUrl() {
	return rpcNodeUrl;
}

byte PWRJ::getChainId() {
	//try {
	//	if (chainId == 255) {
	//		// Make HTTP GET request to retrieve chainId
	//		json object = httpGet(rpcNodeUrl + "/chainId/");

	//		// Extract chainId from JSON response
	//		chainId = (byte)object["chainId"].get<byte>();
	//	}
	//}
	//catch (const std::exception& e) {
	//	throw std::runtime_error("Error fetching chainId: " + std::string(e.what()));
	//}

	return chainId;
}

long PWRJ::getFeePerByte() {
	try {
		if (feePerByte == 0) {
			// Make HTTP GET request to retrieve feePerByte
			json object = httpGet(rpcNodeUrl + "/feePerByte/");

			// Extract feePerByte from JSON response
			feePerByte = object["feePerByte"].get<long>();
		}
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching feePerByte: " + std::string(e.what()));
	}
	return feePerByte;
}

short PWRJ::getBlockchainVersion() {
	try {
		// Make HTTP GET request to retrieve blockchainVersion
		json object = httpGet(PWRJ::rpcNodeUrl + "/blockchainVersion/");
	
		// Extract and return blockchainVersion from JSON response
		return static_cast<short>(object["blockchainVersion"].get<short>());

	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching blockchainVersion: " + std::string(e.what()));
	}
}

int PWRJ::getNonceOfAddress(std::string address) {
	try {
		json object = httpGet(rpcNodeUrl + "/nonceOfUser/?userAddress=" + address);
		int value = object["nonce"].get<int>();
		return value;
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching nonceofaddress: " + std::string(e.what()));
	}
}

int64_t PWRJ::getBalanceOfAddress(std::string address) {
	try {
		json object = httpGet(rpcNodeUrl + "/balanceOf/?userAddress=" + address);
		int64_t value = object["balance"].get<int64_t>();
		return value;
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching balanceofaddress: " + std::string(e.what()));
	}
}

std::string PWRJ::getGuardianOfAddress(std::string address) {
	try {
		json object = httpGet(rpcNodeUrl + "/guardianOf/?userAddress=" + address);

		if (object["isGuarded"].get<bool>()) {
			std::string value = object["guardian"].get<std::string>();
			return value;
		}
		else {
			return "";
		}
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching guardianofaddress: " + std::string(e.what()));
	}
};

long PWRJ::getBlocksCount() {
	try {
		json object = httpGet(rpcNodeUrl + "/blocksCount/");
		long value = object["blocksCount"].get<long>();
		return value;
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching blockcount: " + std::string(e.what()));
	}
}

long PWRJ::getLatestBlockNumber() {
	try {
		return getBlocksCount() - 1;

	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching latestblocknumber: " + std::string(e.what()));
	}
}

Block PWRJ::getBlockByNumber(long blockNumber){
	try {
		json object = httpGet(rpcNodeUrl + "/block/?blockNumber=" + to_string(blockNumber));
		json value = object["block"].get<json>();
		Block block(value);
		return block;
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching getBlockByNumber: " + std::string(e.what()));
	}
}

vector<VmDataTxn> PWRJ::getVMDataTransactions(long startingBlock, long endingBlock, long vmId) {
	try {
		json object = httpGet(rpcNodeUrl + "/getVmTransactions/?startingBlock=" + to_string(startingBlock) + "&endingBlock=" + to_string(endingBlock) + "&vmId=" + to_string(vmId));
		json txns = object["transactions"];
		vector<VmDataTxn> txnsArray;
		txnsArray.reserve(txns.size());


		for (const auto& txnsObject : txns) {
			json txnObjects = txnsObject;
			VmDataTxn txn(txnObjects, txnObjects.value("blockNumber", 0), txnObjects.value("timestamp", 0), txnObjects.value("positionInTheBlock", 0));
			txnsArray.emplace_back(txn);
		}

		return txnsArray;
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching getVMDataTxns: " + std::string(e.what()));
	}
}

vector<VmDataTxn> PWRJ::getVMDataTransactionsFilterByBytePrefix(long startingBlock, long endingBlock, long vmId, vector<byte> prefix) {
	try {
		json object = httpGet(rpcNodeUrl + "/ getVmTransactionsSortByBytePrefix / ? startingBlock =" 
			+ to_string(startingBlock) + "&endingBlock=" + to_string(endingBlock) + "&vmId=" + to_string(vmId)
			+ "&bytePrefix=" + byteArrayToHexString(prefix));
		json txns = object["transactions"];
		vector<VmDataTxn> txnsArray;
		txnsArray.reserve(txns.size());

		for (const auto& txnsObject : txns) {
			json txnObjects = txnsObject;
			VmDataTxn txn(txnObjects, txnObjects.value("blockNumber", 0), txnObjects.value("timestamp", 0), txnObjects.value("positionInTheBlock", 0));
			txnsArray.emplace_back(txn);
		}

		return txnsArray;
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching getVMDataTxns: " + std::string(e.what()));
	}
}

TxnForGuardianApproval PWRJ::isTransactionValidForGuardianApproval(const std::string& txn) {
	try {
		// Create JSON object with transaction data
		json requestData = {
			{"transaction", txn}
		};

		// Make HTTP POST request using httpPost function
		json responseObject = httpPost(rpcNodeUrl + "/isTransactionValidForGuardianApproval/", requestData);
	
		// Check if transaction is valid
		bool valid = responseObject["valid"].get<bool>();
		if (valid) {
			// Transaction is invalid
			return TxnForGuardianApproval{
				true,
				responseObject.value("guardian", "0x"),
				Transaction::fromJSON(responseObject["transaction"], 0, 0, 0),
				//Transaction::Transaction(),
				
			};
		}
		else {
			// Transaction is valid
			return TxnForGuardianApproval{
				false,responseObject.value("guardian", "0x"),
				responseObject["error"].get<std::string>(),
				Transaction::fromJSON(responseObject["transaction"], 0, 0, 0),
				
			};
		}
	}
	catch (const std::exception& e) {
		// Handle exceptions
		throw std::runtime_error("Error in isTransactionValidForGuardianApproval: " + std::string(e.what()));
	}
}

TxnForGuardianApproval PWRJ::isTransactionValidForGuardianApproval(const std::vector<byte>& txn) {
	try {
		return isTransactionValidForGuardianApproval(byteArrayToHexString(txn));
	}
	catch (std::exception& e) {
		throw std::runtime_error("Error: " + std::string(e.what()));
	}
}

int64_t PWRJ::getActiveVotingPower() {
	try {
		json object = httpGet(rpcNodeUrl + "/activeVotingPower/");
		int64_t value = object["activeVotingPower"].get<int64_t>();
		return value;
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching getActivePower: " + std::string(e.what()));
	}
}

int PWRJ::getTotalValidatorsCount() {
	try {
		json object = httpGet(rpcNodeUrl + "/totalValidatorsCount/");
		int value = object["validatorsCount"].get<int>();
		return value;
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching getTotalValidatorsCount: " + std::string(e.what()));
	}
}

int PWRJ::getStandbyValidatorsCount() {
	try {
		json object = httpGet(rpcNodeUrl + "/standbyValidatorsCount/");
		int value = object["validatorsCount"].get<int>();
		return value;
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching getStandbyValidatorsCount: " + std::string(e.what()));
	}
}

int PWRJ::getActiveValidatorsCount() {
	try {
		json object = httpGet(rpcNodeUrl + "/activeValidatorsCount/");
		int value = object["validatorsCount"].get<int>();
		return value;
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching getActiveValidatorsCount: " + std::string(e.what()));
	}

}

int PWRJ::getTotalDelegatorsCount(){
	try {
		json object = httpGet(rpcNodeUrl + "/totalDelegatorsCount/");
		int value = object["delegatorsCount"].get<int>();
		return value;
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching getTotalFelegatorsCount: " + std::string(e.what()));
	}
}

vector<Validator> PWRJ::getAllValidators(){
	try {
		json object = httpGet(rpcNodeUrl + "/allValidators/");
		json validators = object["validators"].get<json>();
		vector<Validator> validatorsList;


		for (const auto& txnsObject : validators) {
			json validatorObject = txnsObject;
			//public Validator(std::string address, std::string ip, boolean badActor, long votingPower, long shares, int delegatorsCount) {
			long votingPower, totalShares;
			if (validatorObject.contains("votingPower")) {
				votingPower = validatorObject["votingPower"].get<int64_t>();
			}
			else {
				votingPower = 0L;
			}

			if (validatorObject.contains("totalShares")) {
				totalShares = validatorObject["totalShares"].get<int64_t>();
			}
			else {
				totalShares = 0L;
			}

			int delegatorsCount;
			if (validatorObject.contains("delegatorsCount")) {
				delegatorsCount = validatorObject["delegatorsCount"].get<int>();
			}
			else {
				delegatorsCount = 0;
			}

			Validator validator(
				validatorObject["address"].get<std::string>(),
				validatorObject["ip"].get<std::string>(),
				validatorObject.value("badActor", false),
				votingPower,
				totalShares,
				delegatorsCount,
				validatorObject.value("status", "unknown"));

			validatorsList.push_back(validator);

		}
		return validatorsList;
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching getAllValidators: " + std::string(e.what()));
	}
}

vector<Validator> PWRJ::getStandbyValidators() {
	try {
		json object = httpGet(rpcNodeUrl + "/standbyValidators/");
		json validators = object["validators"].get<json>();
		vector<Validator> validatorsList;

		for (const auto& txnsObject : validators) {
			json validatorObject = txnsObject;
			//public Validator(std::string address, std::string ip, boolean badActor, long votingPower, long shares, int delegatorsCount) {
			long votingPower, totalShares;
			if (validatorObject.contains("votingPower")) {
				votingPower = validatorObject["votingPower"].get<int64_t>();
			}
			else {
				votingPower = 0L;
			}

			if (validatorObject.contains("totalShares")) {
				totalShares = validatorObject["totalShares"].get<int64_t>();
			}
			else {
				totalShares = 0L;
			}

			int delegatorsCount;
			if (validatorObject.contains("delegatorsCount")) {
				delegatorsCount = validatorObject["delegatorsCount"].get<int>();
			}
			else {
				delegatorsCount = 0;
			}

			Validator validator(
				validatorObject["address"].get<std::string>(),
				validatorObject["ip"].get<std::string>(),
				validatorObject.value("badActor", false),
				votingPower,
				totalShares,
				delegatorsCount,
				"standby");

			validatorsList.push_back(validator);

		}
		return validatorsList;
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching getStandbyValidators: " + std::string(e.what()));
	}
}

vector<Validator> PWRJ::getActiveValidators() {
	try {
		json object = httpGet(rpcNodeUrl + "/activeValidators/");
		json validators = object["validators"].get<json>();
		vector<Validator> validatorsList;

		for (const auto& txnsObject : validators) {
			json validatorObject = txnsObject;
			//public Validator(std::string address, std::string ip, boolean badActor, long votingPower, long shares, int delegatorsCount) {
			long votingPower, totalShares;
			if (validatorObject.contains("votingPower")) {
				votingPower = validatorObject["votingPower"].get<long>();
			}
			else {
				votingPower = 0L;
			}

			if (validatorObject.contains("totalShares")) {
				totalShares = validatorObject["totalShares"].get<long>();
			}
			else {
				totalShares = 0L;
			}

			int delegatorsCount;
			if (validatorObject.contains("delegatorsCount")) {
				delegatorsCount = validatorObject["delegatorsCount"].get<int>();
			}
			else {
				delegatorsCount = 0;
			}

			Validator validator(
				validatorObject["address"].get<std::string>(),
				validatorObject["ip"].get<std::string>(),
				validatorObject.value("badActor", false),
				votingPower,
				totalShares,
				delegatorsCount,
				"active");

			validatorsList.push_back(validator);
		}
		return validatorsList;
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching getActiveValidators: " + std::string(e.what()));
	}
}

vector<Validator> PWRJ::getDelegatees(const std::string& address){
	try {

		json object = httpGet(rpcNodeUrl + "/delegateesOfUser/?userAddress=" + address);
		json validators = object["validators"].get<json>();
		vector<Validator> validatorsList;

		for (const auto& txnsObject : validators) {
			json validatorObject = txnsObject;

			Validator validator(
				validatorObject["address"].get<std::string>(),
				validatorObject["ip"].get<std::string>(),
				validatorObject["badActor"].get<bool>(),
				validatorObject["votingPower"].get<int64_t>(),
				validatorObject["totalShares"].get<int64_t>(),
				validatorObject["delegatorsCount"].get<int>(),
				"active");

			validatorsList.push_back(validator);
		}
		return validatorsList;
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching getDelegatees: " + std::string(e.what()));
	}
}

Validator PWRJ::getValidator(const std::string& validatorAddress) {
	json object = httpGet(rpcNodeUrl + "/validator/?validatorAddress=" + validatorAddress);
	json validatorObject = object["validator"].get<json>();

	Validator validator(
		validatorObject["address"].get<std::string>(),
		validatorObject["ip"].get<std::string>(),
		validatorObject.value("badActor", false),
		validatorObject["votingPower"].get<int64_t>(),
		validatorObject["totalShares"].get<int64_t>(),
		validatorObject["delegatorsCount"].get<int>(),
		validatorObject["status"].get<std::string>());

	return validator;
}

int64_t PWRJ::getDelegatedPWR(const std::string& delegatorAddress, const std::string& validatorAddress){
	try {
		json object = httpGet(rpcNodeUrl + "/validator/delegator/delegatedPWROfAddress/?userAddress=" + delegatorAddress + "&validatorAddress=" + validatorAddress);
		int64_t value = object["delegatedPWR"].get<int64_t>();
		return value;
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching getDelegatedPWR: " + std::string(e.what()));
	}
}

BigDecimal PWRJ::getShareValue(const std::string& validator){
	try {
		json object = httpGet(rpcNodeUrl + "/validator/shareValue/?validatorAddress=" + validator);
		std::string stringValue = object["shareValue"].get<std::string>();
		BigDecimal value(stringValue);
		return value;
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching getShareValue: " + std::string(e.what()));
	}
}

std::string PWRJ::getOwnerOfVm(long vmId){
	try {
		json object = httpGet(rpcNodeUrl + "/ownerOfVmId/?vmId=" + to_string(vmId));
		std::string value = object["owner"].get<std::string>();
		return value;
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching getOwnerOfVm: " + std::string(e.what()));
	}
}

void PWRJ::updateFeePerByte(){
	try {
		json object = httpGet(rpcNodeUrl + "/feePerByte/");
		long feePerByte = object["feePerByte"].get<long>();
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error fetching updateFeePerByte: " + std::string(e.what()));
	}
}

Response PWRJ::broadcastTxn(const vector<byte>& txn) {
	try {
		//string fullHost = "https://" + rpcNodeUrl;
		// Extract the hostname and path from the URL
		std::string hostname;
		std::string path;

		size_t pos = rpcNodeUrl.find("://");
		if (pos != std::string::npos) {
			pos += 3; // Skip past "://"
			size_t end = rpcNodeUrl.find('/', pos);
			if (end != std::string::npos) {
				hostname = rpcNodeUrl.substr(pos, end - pos);
				path = rpcNodeUrl.substr(end);
			}
			else {
				hostname = rpcNodeUrl.substr(pos);
				path = "/";
			}
		}
		else {
			// URL format is invalid
			throw std::runtime_error("Invalid URL format");
		}

	
		// Timeout configuration
		int timeout = 3 * 1000; // 3 seconds in milliseconds

		// Create HTTP client
		httplib::SSLClient client(hostname.c_str());

		// Set request timeout
		client.set_connection_timeout(timeout);
		client.set_read_timeout(timeout);
		client.set_write_timeout(timeout);

		// Create JSON object
		json jsonData;
		jsonData["txn"] = byteArrayToHexString(txn);

		// Create POST request with JSON body and headers
		httplib::Headers headers = {
			{"Accept", "application/json"},
			{"Content-Type", "application/json"}
		};

		auto res = client.Post("/broadcast/", headers, jsonData.dump(), "application/json");


		// Process response
		if (res && res->status == 200) {
			return Response(true, "0x" + byteArrayToHexString(Hash::sha3(txn)), "" );
		}
		else if (res && res->status == 400) {
			json responseObject = json::parse(res->body);
			return Response(false, "", responseObject["message"].get<string>());
		}
		else {
			throw runtime_error("Failed with HTTP error code: " + to_string(res ? res->status : -1));
		}
	}
	catch (const exception& e) {
		return Response(false, "", e.what());
	}
}

template<typename T>
T getOrDefault(const json& json, std::string key, const T& defaultValue) {
	return json.contains(key) ? json.get(key) : defaultValue;
}

//int main() {
//	try {
//		PWRJ("https://pwrrpc.pwrlabs.io");
//		//cout << PWRJ::getRpcNodeUrl() << endl;
//		//cout << toascii(PWRJ::getChainId()) << endl;
//		//cout << PWRJ::getFeePerByte() <<endl;
//		//cout << PWRJ::getBlockchainVersion() << endl;
//		//cout << PWRJ::getNonceOfAddress("1A3F") << endl;
//		/////cout << PWRJ::getBalanceOfAddress("0x61Bd8fc1e30526Aaf1C4706Ada595d6d236d9883") << endl;
//		//cout << PWRJ::getGuardianOfAddress("1A3F") << endl;
//		//cout << PWRJ::getBlocksCount() << endl;
//		//cout << PWRJ::getLatestBlockNumber() << endl;
//		//Block b = PWRJ::getBlockByNumber(17349);
//		//cout << b.getTransactionCount() << endl;
//		//vector<VmDataTxn> v = PWRJ::getVMDataTransactions(17348, 17349, 10023);
//		//cout << v[0].getHash() << endl;
//		//vector<Validator> vec = PWRJ::getStandbyValidators();
//		//cout << vec[1].getIp() << endl;
//		//cout << PWRJ::getStandbyValidatorsCount() << endl;
//		//vector < Validator> vecActive = PWRJ::getActiveValidators();
//		//cout << vecActive[0].getIp() << endl;
//		//cout << PWRJ::getActiveValidatorsCount() << endl;
//		//vector<Validator> vecAll = PWRJ::getAllValidators();
//		//cout << vecAll[0].getIp() << endl;
//		//cout << PWRJ::getTotalValidatorsCount() << endl;
//		////Validator validator = PWRJ::getValidator("0x61Bd8fc1e30526Aaf1C4706Ada595d6d236d9883");
//		////cout << validator.getVotingPower() << endl;
//		////cout << PWRJ::getActiveVotingPower() << endl;
//		//vector<Validator> vecDel = PWRJ::getDelegatees("0x1234567890");
//		//cout << vecDel[0].getIp() << endl;
//		//cout << PWRJ::getTotalDelegatorsCount();
//		//TxnForGuardianApproval TFGA = PWRJ::isTransactionValidForGuardianApproval("0x61Bd8fc1e30526Aaf1C4706Ada595d6d236d9883");
//		//TFGA.getGuardianAddress();
//		cout << "---------------------------------------------------" << endl;
//		/*cout << PWRJ::getDelegatedPWR("0x61Bd8fc1e30526Aaf1C4706Ada595d6d236d9883", "0x61Bd8fc1e30526Aaf1C4706Ada595d6d236d9883") << endl;
//		cout << "---------------------------------------------------" << endl;
//		cout << PWRJ::getOwnerOfVm(10023) << endl;
//		cout << "---------------------------------------------------" << endl;
//		TxnForGuardianApproval tx = PWRJ::isTransactionValidForGuardianApproval("0");
//		cout << tx.getErrorMessage() << endl;*/
//		std::string rpcNodeUrl = "https://pwrrpc.pwrlabs.io";  // Your RPC node URL
//		json requestBody = {
//			{"transaction", "0"}
//		};
//
//		// Make the HTTP POST request
//		json response = PWRJ::httpPost(rpcNodeUrl + "/isTransactionValidForGuardianApproval/", requestBody);
//
//		// Output the response
//		std::cout << "Response: " << response.dump() << std::endl;
//	}
//	catch (const std::exception& e) {
//		std::cerr << "Exception occurred: " << e.what() << std::endl;
//	}
//
//	return 0;
//}


//int main() {
//	PWRJ("https://pwrrpc.pwrlabs.io");
//	//std::string rpcNodeUrl = "https://pwrrpc.pwrlabs.io";
//	std::string transaction = "0"; // Example transaction data
//
//	try {
//		// Make the HTTP POST request
//		TxnForGuardianApproval tx = PWRJ::isTransactionValidForGuardianApproval(transaction);
//
//		// Output the response
//		std::cout << "Transaction is valid: " << tx.getErrorMessage() << std::endl;
//		std::cout << "Guardian address: " << tx.getGuardianAddress() << std::endl;
//		std::cout << "Error message: " << tx.getErrorMessage() << std::endl;
//		// Output other transaction details as needed
//	}
//	catch (const std::exception& e) {
//		// Handle exceptions
//		std::cerr << "Exception occurred: " << e.what() << std::endl;
//	}
//
//	return 0;
//}
//
//int main() {
//	PWRJ("https://pwrrpc.pwrlabs.io");
//	cout << static_cast<int>(PWRJ::getChainId()) << endl;

	//std::vector<byte> byteVector;

	//// Add some bytes to the vector
	//byteVector.push_back(0x12);
	//byteVector.push_back(0xAB);
	//byteVector.push_back(0xFF);

	//Response res = PWRJ::broadcastTxn(byteVector);
	//cout << res.getError() << endl;
