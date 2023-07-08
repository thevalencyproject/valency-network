#ifndef NODETRANSACTIONINFO_H
#define NODETRANSACTIONINFO_H

#include <string>


struct NodeTransactionInfo {
    std::string time;
    std::string date;
    unsigned int numOfVerificationNodes;
    double verificationReward;
};

#endif