#ifndef SYNCACTIVENODES_H
#define SYNCACTIVENODES_H

#include <thread>
#include "local-save/nodes/SaveNodes.h"
#include "valency-core/custom-types/Position.h"
#include "valency-core/networking/client/Client.h"
#include "valency-core/networking/server/Server.h"
#include "valency-core/networking/onion-routing/Onion.h"


struct SyncActiveNodes {
private:
    std::vector<Position3D>* active;

    Server server;
    Client client;
    Onion onion(2);
    SaveNodes save;

    std::string communicate(std::string input);     // Passed into the networking frameworks (client, server, and onion can use same function)

public:
    SyncActiveNodes() {};

    // Runs in its own core - constantly syncs the active nodes: Ensure the  knownNodes vector contains at least 1 active node
    //   -> Designed to take pointers to constantly changing vectors (changed in other threads)
    void sync(std::vector<Position3D>* activeNodes, std::vector<Position3D>* knownNodes);                                // Non Onion-Routing
    void sync(std::vector<Position3D>* activeNodes, std::vector<Position3D>* knownNodes, std::vector<NodeInfo> nodes);   // Onion-Routing - ensure nodes vector only contains onion-routing nodes and not destination server

    void read(std::vector<Position3D>* activeNodes, std::string filePath);    // Also has a file reading/writing functionality through the local-save framework (in valency-network)
    void save(std::vector<Position3D>* activeNodes, std::string filePath);    //   -> filePath should contain the file name / SaveNodes save;
};

#endif