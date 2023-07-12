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
    std::vector<Position3D>* known;
    int nodesRequiredForVerification;    // The number of nodes required for a known node to be added: 50% of all nodes + 1 node

    Server server;
    Client client;
    Onion onion(2);
    SaveNodes save;

    std::string communicate(std::string input);     // Passed into the networking frameworks (client, server, and onion can use same function)

public:
    SyncKnownNodes() {};

    // Runs in its own core - constantly syncs the known nodes: activeNodes does not need to be filled, however knownNodes should contain atleast 1 active node
    //   -> Designed to take pointers to constantly changing vectors (changed in other threads)
    //   -> sync() is for wallets + misc programs and nodeSync() is for nodes
    void sync(std::vector<Position3D>* knownNodes, std::vector<Position3D>* activeNodes);                                      // Non Onion-Routing
    void sync(std::vector<Position3D>* knownNodes, std::vector<Position3D>* activeNodes, std::vector<NodeInfo> nodes);         // Onion-Routing - ensure nodes vector only contains onion-routing nodes and not destination server
    void nodeSync(std::vector<Position3D>* knownNodes, std::vector<Position3D>* activeNodes);                                  // Non Onion-Routing
    void nodeSync(std::vector<Position3D>* knownNodes, std::vector<Position3D>* activeNodes, std::vector<NodeInfo> nodes);     // Onion-Routing - ensure nodes vector only contains onion-routing nodes and not destination server

    void read(std::vector<Position3D>* knownNodes, std::string filePath);    // Also has a file reading/writing functionality through the local-save framework (in valency-network)
    void save(std::vector<Position3D>* knownNodes, std::string filePath);    //   -> filePath should contain the file name / SaveNodes save;
};

#endif