#ifndef SHARD_H
#define SHARD_H

#include <string>
#include <vector>
#include "Block.h"


struct Shard {
    std::vector<Block> shard;     // The blocks that are in the shard - shard creation time can be found by looking at the first block
    std::string shardHash;        // Unique Shard Identifier

    std::string prevShardHash;    // Links to the chain of shards
};

#endif