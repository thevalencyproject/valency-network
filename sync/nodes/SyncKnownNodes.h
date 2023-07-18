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
    /* x: Public Address
       y: IP Address
       z: Port
       i: Bias */
    std::vector<Position4D>* knownnodes;

    unsigned int numOfActiveNodes;                                                      // The number of nodes that are active on the network
    unsigned long bias = 0;                                                             // Node Bias - replicating mutex without mutex
    std::vector<std::pair<std::vector<Position4D>, unsigned long>> unverifiedNodes;     // The knownnodes that some nodes have sent, byt others havent - unsigned long is the node bias
    void validate();                                                                    // Validates the incoming knownnodes

    Server server;
    Client client;
    Onion onion(2);
    SaveNodes save;

    std::string convertNode(Position4D* node);                  // Converts a node into string form - used for network communication
    std::string convertNode(std::vector<Position4D>* nodes);    // Converts a vector of nodes into string form - used for network communication
    Position4D convertString(std::string node);                 // Converts string to a node (string should come from convertNode() function)
    std::vector<Position4D> convertString(std::string nodes);   // Converts string to a vector of nodes (string should come from convertNode() function)

    // Interfaces with the client networking framework to connect to a node and increment numOfActiveNodes
    template<typename T>
    void connectToNode(std::string* ip, int* port, std::string (T::*communicate)(std::string), std::string initialMessage);
    template<typename T>
    void connectToNodeOnion(std::vector<NodeInfo> nodes, std::string (T::*communicate)(std::string), std::string initialMessage);

    std::string communicate(std::string input);         // Passed into the networking frameworks (client, server, and onion can use same function)
    std::string nodeCommunicate(std::string input);     // Node Only: Passed into the networking frameworks (client, server, and onion can use same function)

public:
    SyncKnownNodes() {};

    // Runs in its own core - constantly syncs the known nodes: activeNodes does not need to be filled, however knownNodes should contain atleast 1 active node
    //   -> Designed to take pointers to constantly changing vectors (changed in other threads)
    //   -> sync() is for wallets + misc programs and nodeSync() is for nodes
    void sync(std::vector<Position4D>* knownNodes, std::vector<Position4D>* activeNodes);                                      // Non Onion-Routing
    void sync(std::vector<Position4D>* knownNodes, std::vector<Position4D>* activeNodes, std::vector<NodeInfo> nodes);         // Onion-Routing - ensure nodes vector only contains onion-routing nodes and not destination server
    void nodeSync(std::vector<Position4D>* knownNodes, std::vector<Position4D>* activeNodes);                                  // Non Onion-Routing
    void nodeSync(std::vector<Position4D>* knownNodes, std::vector<Position4D>* activeNodes, std::vector<NodeInfo> nodes);     // Onion-Routing - ensure nodes vector only contains onion-routing nodes and not destination server

    void read(std::vector<Position4D>* knownNodes, std::string filePath);    // Also has a file reading/writing functionality through the local-save framework (in valency-network)
    void save(std::vector<Position4D>* knownNodes, std::string filePath);    //   -> filePath should contain the file name / SaveNodes save;
};

#endif