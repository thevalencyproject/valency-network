#ifndef SYNCKNOWNNODES_H
#define SYNCKNOWNNODES_H

#include <thread>
#include "local-save/nodes/SaveNodes.h"
#include "valency-core/custom-types/Position.h"
#include "valency-core/networking/client/Client.h"
#include "valency-core/networking/server/Server.h"
#include "valency-core/networking/onion-routing/Onion.h"


struct SyncKnownNodes {
private:
    bool onion = false;     // Does not use onion routing by default

    Server server;
    Client client;
    Onion onion(2);

public:
    SyncKnownNodes() {};
    SyncKnownNodes(bool onionRouting) { onion = onionRouting };   // true if communicate using the onion routing framework

    // Runs in its own core - constantly syncs the known nodes: activeNodes does not need to be filled, however knownNodes should contain atleast 1 active node
    //   -> Designed to take pointers to constantly changing vectors (changed in other threads)
    void sync(std::vector<Position3D>* knownNodes, std::vector<Position3D>* activeNodes);

    void read(std::vector<Position3D>* knownNodes, std::string filePath);    // Also has a file reading/writing functionality through the local-save framework (in valency-network)
    void save(std::vector<Position3D>* knownNodes, std::string filePath);    //   -> filePath should contain the file name / SaveNodes save;
};

#endif