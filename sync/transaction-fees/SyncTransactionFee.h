/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-network/blob/main/LICENSE for further license details.        */

#ifndef SYNCTRANSACTIONFEE_H
#define SYNCTRANSACTIONFEE_H

#include <ctime>
#include <chrono>
#include <string>
#include <vector>
#include <thread>
#include <sstream>
#include <iomanip>
#include "structures/Node.h"
#include "configuration/Configuration.h"
#include "valency-core/custom-types/Position.h"
#include "valency-core/networking/client/Client.h"


/* Analyses Chain and Network Data to generate transaction fees
        -> Run on network nodes and wallets (to determine validation rewards (node) + transaction fees (wallet))
        -> Run in its own thread
        -> Updates the transaction fee IF a new block is added AND the previous transaction fee has expired(timed out) */
class SyncTransactionFee {
private:
    std::chrono::duration bandwidthUpdateTimer;         // Update the theoreticalMaxNetworkBandwidth every 10m
    std::vector<unsigned int> nodemaxbandwidths;        // The max bandwidth of each node (of all time) - used to get the theoretical maximum network bandwidth
    std::vector<unsigned int> currentnodebandwidths;    // The current bandwidths of each node
    unsigned int theoreticalMaxNetworkBandwidth;        // [IN MB/s] The theoretical max bandwidth (max bandwidth for single node * numOfActiveNodes)
    unsigned int currentNetworkBandwidth;               // [IN MB/s] The bandwidth of each active node added up

    std::vector<double> transactionfees;   // Contains all transaction fee's for every combination of #ofTransactions & #ofOnionRoutingNodes
    std::stringstream expiry;              // Contains the expiry time for the transaction fee's (do expiry.str() to get string version)

    Client client;
    Configuration config;

    void syncExpiry();                                      // Syncs the expiry time - this increments by 30s every 30s - doesn't need to get current expiry from other nodes because it just increments by 30s every 30s (starting from 0s)
    void syncFee(std::vector<NodeDetails>* knownNodes);     // Syncs the transaction fee - requires Knownnodes list to get the number of active nodes (used to determine network load and therefore fee's)

public:
    SyncTransactionFee() {};

    void sync(std::vector<NodeDetails>* knownNodes);     // Creates and runs its own thread doing sync()   
    
    std::pair<std::string, double> getTransactionFee(int numOfTransactions);                         // Returns the transaction fee and expiry
};

#endif