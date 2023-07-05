#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <string>
#include <vector>
#include "Shard.h"

struct Blockchain {
    std::vector<Shard> chain;
    std::string identifier = "Valency Network Testnet";     // A unique identifier for the network
};

#endif