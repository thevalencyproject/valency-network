#ifndef SYNCTRANSACTIONFEE_H
#define SYNCTRANSACTIONFEE_H

#include <string>
#include <vector>
#include "valency-core/networking/client/Client.h"
#include "valency-core/networking/server/Server.h"
#include "valency-core/networking/onion-routing/Onion.h"


/* Analyses Chain and Network Data to generate transaction fees
        -> Run on network nodes and wallets (to determine validation rewards (node) + transaction fees (wallet))
        -> Run in its own thread
        -> Updates the transaction fee IF a new block is added AND the previous transaction fee has expired(timed out) */
class SyncTransactionFee {
private:
    std::vector<std::pair<std::string, double>>* transactionfees;   // Contains all transaction fee's for every combination of #ofTransactions & #ofOnionRoutingNodes

    Server server;
    Client client;
    Onion onion(2);

public:
    SyncTransactionFee();   // Creates and runs its own thread

    void sync(std::vector<Position4D>* knownNodes);     // Requires Knownnodes list to get the number of active nodes (used to determine network load and therefore fee's)
};

#endif