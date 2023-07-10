#ifndef SYNCBLOCKCHAIN_H
#define SYNCBLOCKCHAIN_H

#include <thread>
#include "structures/Blockchain.h"
#include "valency-core/custom-types/Position.h"
#include "local-save/blockchain/SaveBlockchain.h"
#include "valency-core/networking/client/Client.h"
#include "valency-core/networking/server/Server.h"
#include "valency-core/networking/onion-routing/Onion.h"


struct SyncBlockchain {
private:
    bool onion = false;     // Does not use onion routing by default

    Server server;
    Client client;
    Onion onion(2);

public:
    SyncBlockchain() {};
    SyncBlockchain(bool onionRouting) { onion = onionRouting };   // true if communicate using the onion routing framework

    // Runs in its own core - constantly syncs the blockchain: Ensure the activeNodes vector is also being synced by SyncActiveNodes
    //   -> Designed to take pointers to constantly changing vectors (changed in other threads)
    void sync(Blockchain* blockchain, std::vector<Position3D>* activeNodes);

    void read(Blockchain* blockchain, std::string filePath);    // Also has a file reading/writing functionality through the local-save framework (in valency-network)
    void save(Blockchain* blockchain, std::string filePath);    //   -> filePath should contain the file name / SaveBlockchain save;
};

#endif