/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-network/blob/main/LICENSE for further license details.        */

#ifndef SYNCKNOWNNODES_H
#define SYNCKNOWNNODES_H

#include <thread>
#include "structures/Node.h"
#include "local-save/nodes/SaveNodes.h"
#include "configuration/Configuration.h"
#include "valency-core/custom-types/Position.h"
#include "valency-core/networking/client/Client.h"
#include "valency-core/networking/server/Server.h"
#include "valency-core/networking/onion-routing/Onion.h"
#include "valency-core/algorithms/lzma-compression/LZMA-Compression.h"


class SyncKnownNodes {
private:
    /* x: Public Address
       y: IP Address
       z: Port
       i: Bias
       j: Network Node Bandwidth Vector */
    std::vector<NodeDetails>* knownnodes;
    Configuration* configuration;                  // A pointer to the network configuration

    int numberOfDefaultNodes = 5;                                                       // The number of nodes that are included in the program by default
    unsigned int numOfActiveNodes;                                                      // The number of nodes that are active on the network
    unsigned long bias = 0;                                                             // Node Bias - replicating mutex without mutex
    std::vector<std::pair<std::vector<NodeDetails>, unsigned long>> unverifiedNodes;     // The knownnodes that some nodes have sent, but others havent - unsigned long is the node bias
    void validate();                                                                    // Validates the incoming knownnodes

    Server server;
    Client client;
    Onion onion(2);
    SaveNodes save;
    LZMACompression lzma;

    std::string convertNode(NodeDetails* node);                  // Converts a node into string form - used for network communication
    std::string convertNode(std::vector<NodeDetails>* nodes);    // Converts a vector of nodes into string form - used for network communication
    NodeDetails convertString(std::string node);                 // Converts string to a node (string should come from convertNode() function)
    std::vector<NodeDetails> convertString(std::string nodes);   // Converts string to a vector of nodes (string should come from convertNode() function)

    // Interfaces with the client networking framework to connect to a node and increment numOfActiveNodes
    template<typename T>
    void connectToNode(std::string* ip, int* port, std::string (T::*communicate)(std::string), std::string initialMessage);
    template<typename T>
    void connectToNodeOnion(std::vector<NodeInfo> nodes, std::string (T::*communicate)(std::string), std::string initialMessage);

    std::string communicate(std::string input);         // Passed into the networking frameworks (client, server, and onion can use same function)
    std::string nodeCommunicate(std::string input);     // Node Only: Passed into the networking frameworks (client, server, and onion can use same function)

public:
    SyncKnownNodes(Configuration* config) { configuration = config; };

    // Runs in its own core - constantly syncs the known nodes: knownNodes should contain atleast 1 active node
    //   -> Designed to take pointers to constantly changing vectors (changed in other threads)
    //   -> sync() is for wallets + misc programs and nodeSync() is for nodes
    void sync(std::vector<NodeDetails>* knownNodes);                                      // Non Onion-Routing
    void sync(std::vector<NodeDetails>* knownNodes, std::vector<NodeInfo> nodes);         // Onion-Routing - ensure nodes vector only contains onion-routing nodes and not destination server
    void nodeSync(std::vector<NodeDetails>* knownNodes);                                  // Non Onion-Routing
    void nodeSync(std::vector<NodeDetails>* knownNodes, std::vector<NodeInfo> nodes);     // Onion-Routing - ensure nodes vector only contains onion-routing nodes and not destination server

    void read(std::vector<NodeDetails>* knownNodes, std::string filePath);    // Also has a file reading/writing functionality through the local-save framework (in valency-network)
    void save(std::vector<NodeDetails>* knownNodes, std::string filePath);    //   -> filePath should contain the file name / SaveNodes save;
};

#endif