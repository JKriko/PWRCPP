#pragma once

#include <iostream>
#include <vector>
#include "Validator.h"
#include "../Protocol/PWRJ.h"

//class PWRJ;

using json = nlohmann::json;
using namespace std;



Validator::Validator(const string& address, const string& ip, bool isBadActor, int64_t votingPower, int64_t shares, int delegatorsCount, const string& status)
	: address(address),
	ip(ip),
	isBadActor(isBadActor),
	votingPower(votingPower),
	shares(shares),
	delegatorsCount(delegatorsCount),
	status(status) {}



const string& Validator::getAddress() const {
	return address;
}

const string& Validator::getIp() const {
	return ip;
}

const bool Validator::getIsBadActor() const {
	return isBadActor;
}

const int64_t Validator::getVotingPower() const {
	return votingPower;
}

const int64_t Validator::getShares() const {
	return shares;
}

const int Validator::getDelegatorsCount() const {
	return delegatorsCount;
}

const string& Validator::getStatus() const {
	return status;
}

vector<Delegator> Validator::getDelegators(const PWRJ& pwrj) {
	try {
		// Extract the hostname and path from the URL
		std::string hostname;
		//std::string path;

		size_t pos = (PWRJ::getRpcNodeUrl()).find("://");
		if (pos != std::string::npos) {
			pos += 3; // Skip past "://"
			size_t end = (PWRJ::getRpcNodeUrl()).find('/', pos);
			if (end != std::string::npos) {
				hostname = (PWRJ::getRpcNodeUrl()).substr(pos, end - pos);
				//path = (PWRJ::getRpcNodeUrl()).substr(end);
			}
			else {
				hostname = (PWRJ::getRpcNodeUrl()).substr(pos);
				//path = "/";
			}
		}
		else {
			// URL format is invalid
			throw std::runtime_error("Invalid URL format");
		}


		// Create SSLClient with just the hostname
		httplib::SSLClient client(hostname.c_str());

		//httplib::Client client(PWRJ::getRpcNodeUrl().c_str());

		auto response = client.Get(("/validator/delegatorsOfValidator/?validatorAddress=" + Validator::address).c_str());
		if (response && response->status == 200) {
			json object = json::parse(response->body);
			string jsonString = object.dump();
			cout << jsonString << endl;

			json delegators = object["delegators"];

			vector<Delegator> delegatorsList;

			for (auto it = delegators.begin(); it != delegators.end(); ++it) {
				string delegatorAddress = it.key();
				long shares = delegators.value(delegatorAddress, 0);

				mp::cpp_int sharesB(shares);
				mp::cpp_int votingPowerB(votingPower);
				mp::cpp_int delegatedPWRB = (sharesB * votingPowerB) / sharesB; // Use cpp_int for intermediate calculations

			// Convert delegatedPWRB to long if necessary (or use cpp_int directly)
				long delegatedPWR = delegatedPWRB.convert_to<long>();
				Delegator d(delegatorAddress, Validator::address, shares, delegatedPWR);

				delegatorsList.push_back(d);
			}

		}
		else if (response && response->status == 400) {
			json object = json::parse(response->body);
			throw std::runtime_error("Failed with HTTP error 400 and message: " + object["message"]);
		}
		else {
			throw std::runtime_error("Failed with HTTP error code: " + std::to_string(response ? response->status : -1));
		}
	}
	catch (const std::exception& e) {
		cout << e.what() << endl;
		return vector<Delegator>();
	}
}


//int main() {
//
//}

