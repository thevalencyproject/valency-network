/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-network/blob/main/LICENSE for further license details.        */

#include "Sync.h"


Sync::Sync(bool node, std::string configurationPath, std::string blockchainPath, std::string knownNodesPath) {     // Non Onion-Routing
    // Set file paths
    configurationFilepath = configurationPath;
    blockchainFilepath = blockchainPath;
    knownNodesFilepath = knownNodesPath;

    // Read each file
    readBlockchain(); readKnownNodes();

    // Continuously Sync (these open in new threads)
    switch(node) {
    case 0:
        syncKnownNodes.sync(knownNodes);
        syncBlockchain.sync(blockchain, knownNodes);
        syncBandwidth.sync(bandwidth, knownNodes);
        break;
    case 1:
        syncKnownNodes.nodeSync(knownNodes);
        syncBlockchain.nodeSync(blockchain, knownNodes);
        syncBandwidth.nodeSync(bandwidth, knownNodes);
        break;
    }
}

Sync::Sync(bool node, std::string configurationPath, std::string blockchainPath, std::string knownNodesPath, std::vector<NodeInfo> nodes) {    // Onion Routing
    // Set file paths
    configurationFilepath = configurationPath;
    blockchainFilepath = blockchainPath;
    knownNodesFilepath = knownNodesPath;

    // Read each file
    readConfiguration(); readBlockchain(); readKnownNodes();

    // Continuously Sync (these open in new threads) - uses the onion-routing function versions
    switch(node) {
    case 0:
        syncKnownNodes.sync(knownNodes, nodes);
        syncBlockchain.sync(blockchain, knownNodes, nodes);
        syncBandwidth.sync(bandwidth, knownNodes, nodes);
        break;
    case 1:
        syncKnownNodes.nodeSync(knownNodes, nodes);
        syncBlockchain.nodeSync(blockchain, knownNodes, nodes);
        syncBandwidth.nodeSync(bandwidth, knownNodes, nodes);
        break;
    }
}

Sync::~Sync() { saveBlockchain(); saveKnownNodes(); }     // Save the data to file

void Sync::readConfiguration(std::string filepath) { saveConfiguration.read(configuration, configurationFilepath); }
void Sync::readBlockchain() { syncBlockchain.read(blockchain, blockchainFilepath); }
void Sync::readKnownNodes() { syncKnownNodes.read(knownNodes, knownNodesFilepath); }
void Sync::saveConfiguration(std::string filepath) { saveConfiguration.save(configuration, configurationFilepath) }
void Sync::saveBlockchain() { syncBlockchain.save(blockchain, blockchainFilepath); }
void Sync::saveKnownNodes() { syncKnownNodes.save(knownNodes, knownNodesFilepath); }



