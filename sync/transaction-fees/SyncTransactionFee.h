#ifndef SYNCTRANSACTIONFEE_H
#define SYNCTRANSACTIONFEE_H

#include <ctime>
#include <chrono>
#include <string>
#include <vector>
#include <thread>
#include <sstream>
#include <iomanip>
#include "valency-core/custom-types/Position.h"
#include "valency-core/networking/client/Client.h"
#include "valency-core/networking/server/Server.h"
#include "valency-core/networking/onion-routing/Onion.h"


/* Analyses Chain and Network Data to generate transaction fees
        -> Run on network nodes and wallets (to determine validation rewards (node) + transaction fees (wallet))
        -> Run in its own thread
        -> Updates the transaction fee IF a new block is added AND the previous transaction fee has expired(timed out) */
class SyncTransactionFee {
private:
    std::vector<double> transactionfees;   // Contains all transaction fee's for every combination of #ofTransactions & #ofOnionRoutingNodes
    std::stringstream expiry;              // Contains the expiry time for the transaction fee's (do expiry.str() to get string version)

    Server server;
    Client client;
    Onion onion(2);

    void syncExpiry();    // Syncs the expiry time - this increments by 30s every 30s - doesn't need to get current expiry from other nodes because it just increments by 30s every 30s (starting from 0s)

public:
    SyncTransactionFee(std::vector<Position4D>* knownNodes);   // Creates and runs its own thread doing sync()

    void sync(std::vector<Position4D>* knownNodes);     // Requires Knownnodes list to get the number of active nodes (used to determine network load and therefore fee's)
    
    std::pair<std::string, double> getTransactionFee(int numOfTransactions);                         // Returns the transaction fee and expiry
    std::pair<std::string, double> getTransactionFee(int numOfTransactions, int numOfOnionNodes);    // Returns the transaction fee and expiry
};

#endif