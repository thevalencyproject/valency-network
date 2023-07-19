#ifndef SYNC_H
#define SYNC_H

#include "blockchain/SyncBlockchain.h"
#include "nodes/SyncKnownNodes.h"


struct Sync {
    // The objects that are synced and their file paths
    Blockchain blockchain;
    std::vector<Position4D> knownNodes;
    std::string blockchainFilepath;
    std::string knownNodesFilepath;

    // The syncing frameworks for each object
    SyncBlockchain syncBlockchain;
    SyncKnownNodes syncKnownNodes;

    // Starts syncing the blockchain, activeNodes, and knownNodes on construction
    //  -> Input whether it is running as a node, or as a wallet (or anything else for that matter)
    Sync(bool node, std::string blockchainPath, std::string knownNodesPath);
    ~Sync();    // Saves the objects to their respective filepaths

    // Passthrough Functions for reading and writing (funnel down to local-save framework)
    void readBlockchain(std::string filepath);
    void readKnownNodes(std::string filepath);
    void saveBlockchain(std::string filepath);
    void saveKnownNodes(std::string filepath);
};

#endif