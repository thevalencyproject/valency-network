#ifndef SYNC_H
#define SYNC_H

#include "blockchain/SyncBlockchain.h"
#include "nodes/SyncActiveNodes.h"
#include "nodes/SyncKnownNodes.h"


struct Sync {
    // The objects that are synced and their file paths
    Blockchain blockchain;
    std::vector<Position3D> activeNodes;
    std::vector<Position3D> knownNodes;
    std::string blockchainFilepath;
    std::string activeNodesFilepath;
    std::string knownNodesFilepath;

    // The syncing frameworks for each object
    SyncBlockchain syncBlockchain;
    SyncActiveNodes syncActiveNodes;
    SyncKnownNodes syncKnownNodes;

    // Starts syncing the blockchain, activeNodes, and knownNodes on construction
    Sync(std::string blockchainPath, std::string activeNodesPath, std::string knownNodesPath);
    ~Sync();    // Saves the objects to their respective filepaths

    // Passthrough Functions for reading and writing (funnel down to local-save framework)
    void readBlockchain(std::string filepath);
    void readActiveNodes(std::string filepath);
    void readKnownNodes(std::string filepath);
    void saveBlockchain(std::string filepath);
    void saveActiveNodes(std::string filepath);
    void saveKnownNodes(std::string filepath);
};

#endif