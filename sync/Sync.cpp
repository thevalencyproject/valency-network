#include "Sync.h"


Sync::Sync(bool node, std::string blockchainPath, std::string activeNodesPath, std::string knownNodesPath) {
    // Set file paths
    blockchainFilepath = blockchainPath;
    activeNodesFilepath = activeNodesPath;
    knownNodesFilepath = knownNodesPath;

    // Read each file
    readBlockchain(); readActiveNodes(); readKnownNodes();

    // Continuously Sync (these open in new threads)
    switch(node) {
    case 0:
        syncActiveNodes.sync(activeNodes, knownNodes);
        syncKnownNodes.sync(knownNodes, activeNodes);
        syncBlockchain.sync(blockchain, activeNodes);
        break;
    case 1:
        syncActiveNodes.nodeSync(activeNodes, knownNodes);
        syncKnownNodes.nodeSync(knownNodes, activeNodes);
        syncBlockchain.nodeSync(blockchain, activeNodes);
        break;
    }
    
}

Sync::~Sync() { saveBlockchain(); saveBlockchain(); saveBlockchain(); }     // Save the data to file

void Sync::readBlockchain() { syncBlockchain.read(blockchain, blockchainFilepath); }
void Sync::readActiveNodes() { syncActiveNodes.read(activeNodes, activeNodesFilepath); }
void Sync::readKnownNodes() { syncKnownNodes.read(knownNodes, knownNodesFilepath); }
void Sync::saveBlockchain() { syncBlockchain.save(blockchain, blockchainFilepath); }
void Sync::saveActiveNodes() { syncActiveNodes.save(activeNodes, activeNodesFilepath); }
void Sync::saveKnownNodes() { syncKnownNodes.save(knownNodes, knownNodesFilepath); }