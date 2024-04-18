#include <string>
#include <iostream>


using namespace std;

class Delegator {
private:
    string address;
    string validatorAddress;
    long shares;
    long delegatedPWR;

public:
    Delegator(const std::string& address, const std::string& validatorAddress, long shares, long delegatedPWR);

    // Getters

    /**
     * @return the address of the delegator
     */
    const std::string& getAddress() const;

    /**
     * @return the address of the validator
     */
    const std::string& getValidatorAddress() const;

    /**
     * @return the shares of the delegator
     */
    long getShares() const;

    /**
     * @return the delegated PWR of the delegator
     */
    long getDelegatedPWR() const;
};

