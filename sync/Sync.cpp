#include "Sync.h"


Sync::Sync(bool node, std::string blockchainPath, std::string knownNodesPath) {
    // Set file paths
    blockchainFilepath = blockchainPath;
    knownNodesFilepath = knownNodesPath;

    // Read each file
    readBlockchain(); readKnownNodes();

    // Continuously Sync (these open in new threads)
    switch(node) {
    case 0:
        syncKnownNodes.sync(knownNodes);
        syncBlockchain.sync(blockchain, knownNodes);
        break;
    case 1:
        syncKnownNodes.nodeSync(knownNodes);
        syncBlockchain.nodeSync(blockchain, knownNodes);
        break;
    }
    
}

Sync::~Sync() { saveBlockchain(); saveKnownNodes(); }     // Save the data to file

void Sync::readBlockchain() { syncBlockchain.read(blockchain, blockchainFilepath); }
void Sync::readKnownNodes() { syncKnownNodes.read(knownNodes, knownNodesFilepath); }
void Sync::saveBlockchain() { syncBlockchain.save(blockchain, blockchainFilepath); }
void Sync::saveKnownNodes() { syncKnownNodes.save(knownNodes, knownNodesFilepath); }