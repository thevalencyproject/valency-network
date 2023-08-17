/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-network/blob/main/LICENSE for further license details.        */

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