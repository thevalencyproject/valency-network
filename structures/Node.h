#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include "structures/Block.h"


struct NodeDetails {
    std::string address;        // The public address that is used to encrypt data sent to this node
    std::string ip;             // The networking IP address to connect to this node
    int port;                   // The networking port to connect to this node
    unsigned long bias;         // The power the node has over the network
};

struct OtherNode {
    NodeDetails details;

    std::vector<std::vector<Block> unverifiedBlocks;            // The unverified blocks on the network (according to this node)
    std::vector<NodeDetails> othersDetails;                     // The details of the other nodes on the network (according to this node)
    std::vector<std::vector<unsigned short>> othersStake;       // The stake of the other nodes on the network (according to this node)
    std::vector<std::vector<unsigned short>> othersBandwidth;   // The bandwidth of the other nodes on the network (according to this node)
};

#endif