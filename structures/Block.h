#ifndef BLOCK_H
#define BLOCK_H

#include <chrono>
#include <string>
#include <vector>
#include "valency-core/cryptography/traceble-ring-signatures/Ring-Signature.h"


struct Block {
    RingSignature ringsignature;                                // The Traceable Ring Signature (contains transaction information)
    std::chrono::time_point<std::chrono::system_clock> time;    // Time the block was created
    std::string blockHash;                                      // Unique block identifier

    std::string prevBlockHash;                                  // Links to the chain of blocks

    void generateBlockHash();   // Generates a block hash from the ring signature and previous block hash
};

#endif