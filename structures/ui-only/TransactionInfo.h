/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-core/blob/main/LICENSE for further license details.        */

#ifndef TRANSACTIONINFO_H
#define TRANSACTIONINFO_H

#include <string>


struct TransactionInfo {
    std::string time;
    std::string date;
    std::string sender;
    std::string receiver;

    unsigned int amount;    // The amount (in the smallest of formats - 0.000001)
    unsigned int fee;       // The fee amount (in the smallest of formats - 0.000001)
    unsigned int nodes;     // The number of onion-routing nodes that were used

    unsigned int balance;   // Wallet Balance after the transaction has gone through
};

#endif