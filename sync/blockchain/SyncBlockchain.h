#ifndef SYNCBLOCKCHAIN_H
#define SYNCBLOCKCHAIN_H

#include <thread>
#include "structures/Blockchain.h"
#include "valency-core/custom-types/Position.h"
#include "local-save/blockchain/SaveBlockchain.h"
#include "valency-core/networking/client/Client.h"
#include "valency-core/networking/server/Server.h"
#include "valency-core/networking/onion-routing/Onion.h"
#include "valency-core/algorithms/lzma-compression/LZMA-Compression.h"


class SyncBlockchain {
private:
    Blockchain* chain;
    
    unsigned int numOfActiveNodes;                                                 // The number of nodes that are active on the network
    unsigned long bias = 0;                                                        // Node Bias - replicating mutex without mutex
    std::vector<std::pair<std::vector<Block>, unsigned long>> unverifiedBlocks;    // The blocks that some nodes have sent, but others havent (not 100% verified yet) - the unsigned long is the node bias
    void validate();                                                               // Validates the incoming blocks from the nodes

    Server server;
    Client client;
    Onion onion(2);
    SaveBlockchain save;
    LZMACompression lzma;

    std::string convertBlock(Block* block);     // Converts a block into string form - used for network communication
    std::string convertBlock(std::vector<Block>* blocks);   // Converts a vector of blocks into string form - used for network communication
    Block convertString(std::string block);                 // Converts string to a block (string should come from convertBlock() function)
    std::vector<Block> convertString(std::string blocks);   // Converts string to a vector of blocks (string should come from convertBlock() function)

    // Interfaces with the client networking framework to connect to a node and increment numOfActiveNodes
    template<typename T>
    void connectToNode(std::string* ip, int* port, std::string (T::*communicate)(std::string), std::string initialMessage);
    template<typename T>
    void connectToNodeOnion(std::vector<NodeInfo> nodes, std::string (T::*communicate)(std::string), std::string initialMessage);

    std::string communicate(std::string input);         // Passed into the networking frameworks (client, server, and onion can use same function)
    std::string nodeCommunicate(std::string input);     // Node Only: Passed into the networking frameworks (client, server, and onion can use same function)

public:
    SyncBlockchain() {};

    // Runs in its own core - constantly syncs the blockchain: Ensure the activeNodes vector is also being synced by SyncActiveNodes
    //   -> Designed to take pointers to constantly changing vectors (changed in other threads)
    //   -> sync() is for wallets + misc programs and nodeSync() is for nodes
    void sync(Blockchain* blockchain, std::vector<Position4D>* activeNodes);                                    // Non Onion-Routing
    void sync(Blockchain* blockchain, std::vector<Position4D>* activeNodes, std::vector<NodeInfo> nodes);       // Onion-Routing - ensure nodes vector only contains onion-routing nodes and not destination server
    void nodeSync(Blockchain* blockchain, std::vector<Position4D>* activeNodes);                                // Non Onion-Routing
    void nodeSync(Blockchain* blockchain, std::vector<Position4D>* activeNodes, std::vector<NodeInfo> nodes);   // Onion-Routing - ensure nodes vector only contains onion-routing nodes and not destination server

    void read(Blockchain* blockchain, std::string filePath);    // Also has a file reading/writing functionality through the local-save framework (in valency-network)
    void save(Blockchain* blockchain, std::string filePath);    //   -> filePath should contain the file name / SaveBlockchain save;
};

#endif