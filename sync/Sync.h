/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-network/blob/main/LICENSE for further license details.        */

#ifndef SYNC_H
#define SYNC_H

#include "nodes/SyncKnownNodes.h"
#include "bandwidth/SyncBandwidth.h"
#include "blockchain/SyncBlockchain.h"
#include "local-save/configuration/SaveConfiguration.h"


struct Sync {
    // The objects that are synced and their file paths
    Blockchain blockchain;
    Configuration configuration;
    std::vector<Position4D> knownNodes;
    std::vector<unsigned short> bandwidth;
    std::string blockchainFilepath;
    std::string knownNodesFilepath;
    std::string configurationFilepath;

    // The syncing/save frameworks for each object
    SyncBlockchain syncBlockchain(&configuration);
    SyncKnownNodes syncKnownNodes(&configuration);
    SyncBandwidth syncBandwidth(&configuration);
    SaveConfiguration saveConfiguration;                // Network Configuration is not synced

    // Starts syncing the blockchain, activeNodes, and knownNodes on construction
    //  -> Input whether it is running as a node, or as a wallet (or anything else for that matter)
    Sync(bool node, std::string configurationPath, std::string blockchainPath, std::string knownNodesPath);                                // Non Onion-Routing
    Sync(bool node, std::string configurationPath, std::string blockchainPath, std::string knownNodesPath, std::vector<NodeInfo> nodes);   // Onion-Routing - nodes should include the selected onion routing nodes
    ~Sync();    // Saves the objects to their respective filepaths

    // Passthrough Functions for reading and writing (funnel down to local-save framework)
    void readConfiguration(std::string filepath);
    void readBlockchain(std::string filepath);
    void readKnownNodes(std::string filepath);
    void saveConfiguration(std::string filepath);
    void saveBlockchain(std::string filepath);
    void saveKnownNodes(std::string filepath);
};

#endif