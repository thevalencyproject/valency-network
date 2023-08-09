#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include "structures/Block.h"


struct NodeDetails {
    std::string address;        // The public address that is used to encrypt data sent to this node
    std::string ip;             // The networking IP address to connect to this node
    int port;                   // The networking port to connect to this node
    unsigned long bandwidth;    // The bandwidth of this node
    unsigned long bias;         // The power the node has over the network
};

struct NodeSync {
    // Used to sync
    std::vector<std::vector<Block>> unverifiedBlocks;           // The unverified blocks on the network (according to this node)
    std::vector<std::pair<std::vector<Block>, unsigned short count>> unverifiedBlocks;

    std::vector<std::vector<NodeDetails>> othersDetails;        // The details of the nodes on the network (according to this node)

    // Used to calculate node bias -> unsigned short = 2 bytes * numOfActiveNodes^2 = amount of space each of these vectors use (e.g. 1000 nodes = 2 megabytes)
    std::vector<std::vector<unsigned short>> othersStake;       // The stake of the other nodes on the network (according to this node)
    std::vector<std::vector<unsigned short>> othersBandwidth;   // The bandwidth of the other nodes on the network (according to this node)
};

struct Client {
    NodeSync sync;

    Blockchain blockchain;
    Configuration configuration;
    std::vector<NodeDetails> knownnodes;
};

#endif