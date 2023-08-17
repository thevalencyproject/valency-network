/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-network/blob/main/LICENSE for further license details.        */

#ifndef SHARD_H
#define SHARD_H

#include <chrono>
#include <string>
#include <vector>
#include "Block.h"
#include "valency-core/cryptography/sha-hashing/SHA256.h"


struct Shard {
    SHA256 sha;

    std::vector<Block> shard;     // The blocks that are in the shard - shard creation time can be found by looking at the first block
    std::string shardHash;        // Unique Shard Identifier - this is a hash of the first block of the hash's hash!

    std::string prevShardHash;    // Links to the chain of shards

    Shard(Block initialBlock, std::string prevHash) {
        shard.push_back(initialBlock);
        prevShardHash = prevHash;
        shardHash = sha.hash(initialBlock.blockHash.c_str());
    };

    // Add a block to the shard - takes in the block you want to add + the maximum shard size: returns a new shard if the shard is filled
    Shard addBlock(Block block, unsigned int maxShardSize) {
        if(shard.size() == maxShardSize) {
            Shard output(block, shardHash);
            return output;
        }

        shard.push_back(block);
    };
};

#endif