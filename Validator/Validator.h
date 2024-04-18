#pragma once

#include <iostream>
#include<list>
#include<vector>
#include "../json/single_include/nlohmann/json.hpp"
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "../cpp-httplib/httplib.h"
#include "../Delegator/Delegator.h"
#include "boost/multiprecision/cpp_int.hpp"


using namespace std;
namespace mp = boost::multiprecision;
using BigInteger = mp::cpp_int;

class PWRJ;

class Validator {
private:
    const string address;
    const string ip;
    const bool isBadActor;
    const int64_t votingPower;
    const int64_t shares;
    const int delegatorsCount;
    const string status;

public:
    Validator(const string& address, const string& ip, bool isBadActor, int64_t votingPower, int64_t shares, int delegatorsCount, const string& status);

    vector<Delegator> getDelegators(const PWRJ& pwrj);

    const string& getAddress() const;
    const string& getIp() const;
    const bool getIsBadActor() const;
    const int64_t getVotingPower() const;
    const int64_t getShares() const;
    const int getDelegatorsCount() const;
    const string& getStatus() const;

};

