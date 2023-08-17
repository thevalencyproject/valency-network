/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-network/blob/main/LICENSE for further license details.        */

#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <string>
#include <vector>
#include "Shard.h"

struct Blockchain {
    std::vector<Shard> chain;
    
    unsigned int length;                            // The shard length
    unsigned int numOfShards;                       // The number of shards in the chain
    std::string id = "Valency Network Testnet";     // A unique identifier for the network

    // initialShard is a blank shard - created with its own unique hash - initialBlock is the first block of the second shard
    Blockchain(std::string identifier, unsigned int shardLength, Shard initialShard, Block initialBlock) {
        id = identifier;
        length = shardLength;
        addShard(initialShard);
        addBlock(initialBlock);
    };

    void addShard(Shard shard) { chain.push_back(shard); numOfShards++; };
    void addBlock(Block block) {
        Shard new = chain.back().addBlock(block, length);
        if(new.prevShardHash.empty())
            return;

        addShard(new);
    };
};

#endif