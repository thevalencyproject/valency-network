#ifndef SYNCTRANSACTIONFEE_H
#define SYNCTRANSACTIONFEE_H

#include <string>
#include <vector>
#include <thread>
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
    std::string expiry;                    // Contains the expiry time for the transaction fee's

    Server server;
    Client client;
    Onion onion(2);

public:
    SyncTransactionFee(std::vector<Position4D>* knownNodes);   // Creates and runs its own thread doing sync()

    void sync(std::vector<Position4D>* knownNodes);     // Requires Knownnodes list to get the number of active nodes (used to determine network load and therefore fee's)
    
    std::pair<std::string, double> getTransactionFee(int numOfTransactions);                                            // Returns the transaction fee and expiry
    std::pair<std::string, double> getTransactionFee(int numOfTransactions, bool onionRouting, int numOfOnionNodes);    // Returns the transaction fee and expiry
};

#endif