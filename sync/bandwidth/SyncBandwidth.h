#ifndef SYNCBANDWIDTH_H
#define SYNCBANDWIDTH_H

#include <thread>
#include "structures/Node.h"
#include "configuration/Configuration.h"
#include "valency-core/networking/client/Client.h"
#include "valency-core/networking/server/Server.h"
#include "valency-core/networking/onion-routing/Onion.h"


class SyncBandwidth {
private:
    std::vector<unsigned short> activebandwidth;     // The bandwidth value for every node - index corresponds to knownnode index
    Configuration* configuration;                    // A pointer to the network configuration
    
    unsigned int numOfActiveNodes;                                                             // The number of nodes that are active on the network
    unsigned long bias = 0;                                                                    // Node Bias - replicating mutex without mutex
    std::vector<std::pair<std::vector<unsigned short>, unsigned long>> unverifiedbandwidth;    // The bandwidth that some nodes have sent, but others havent (not 100% verified yet)
    void validate();                                                                           // Validates the incoming bandwidth from the nodes

    Server server;
    Client client;
    Onion onion(2);

    std::string convertBandwidth(unsigned short* bandwidth);                // Converts a block into string form - used for network communication
    std::string convertBandwidth(std::vector<unsigned short>* bandwidth);   // Converts a vector of blocks into string form - used for network communication
    unsigned short convertString(std::string bandwidth);                    // Converts string to a block (string should come from convertBlock() function)
    std::vector<unsigned short> convertString(std::string bandwidth);       // Converts string to a vector of blocks (string should come from convertBlock() function)

    // Interfaces with the client networking framework to connect to a node and increment numOfActiveNodes
    template<typename T>
    void connectToNode(std::string* ip, int* port, std::string (T::*communicate)(std::string), std::string initialMessage);
    template<typename T>
    void connectToNodeOnion(std::vector<NodeInfo> nodes, std::string (T::*communicate)(std::string), std::string initialMessage);

    std::string communicate(std::string input);         // Passed into the networking frameworks (client, server, and onion can use same function)
    std::string nodeCommunicate(std::string input);     // Node Only: Passed into the networking frameworks (client, server, and onion can use same function)

public:
    SyncBandwidth(Configuration* config) { configuration = config; };

    // Runs in its own core - constantly syncs node bandwidth: Ensure the knownnodes vector is up to date
    //   -> Designed to take pointers to constantly changing vectors (changed in other threads)
    //   -> sync() is for wallets + misc programs and nodeSync() is for nodes
    void sync(std::vector<unsigned short>* bandwidth, std::vector<NodeDetails>* knownNodes);                                    // Non Onion-Routing
    void sync(std::vector<unsigned short>* bandwidth, std::vector<NodeDetails>* knownNodes, std::vector<NodeInfo> nodes);       // Onion-Routing - ensure nodes vector only contains onion-routing nodes and not destination server
    void nodeSync(std::vector<unsigned short>* bandwidth, std::vector<NodeDetails>* knownNodes);                                // Non Onion-Routing
    void nodeSync(std::vector<unsigned short>* bandwidth, std::vector<NodeDetails>* knownNodes, std::vector<NodeInfo> nodes);   // Onion-Routing - ensure nodes vector only contains onion-routing nodes and not destination server
};

#endif