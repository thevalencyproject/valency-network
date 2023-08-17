/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-core/blob/main/LICENSE for further license details.        */

#ifndef BLOCK_H
#define BLOCK_H

#include <chrono>
#include <string>
#include <vector>
#include "valency-core/cryptography/sha-hashing/SHA256.h"
#include "valency-core/cryptography/traceble-ring-signatures/Ring-Signature.h"


struct Block {
    SHA256 sha;

    RingSignature ringsignature;                                // The Traceable Ring Signature (contains transaction information)
    std::chrono::time_point<std::chrono::system_clock> time;    // Time the block was created
    std::string blockHash;                                      // Unique block identifier

    std::string prevBlockHash;                                  // Links to the chain of blocks

    Block(RingSignature signature, std::chrono::time_point<std::chrono::system_clock> blockTime, std::string prevHash) {
        ringsignature = signature;
        time = blockTime;
        prevBlockHash = prevHash;
    }

    // Generates a block hash from the ring signature and previous block hash
    void generateBlockHash() { blockHash = sha.hash((prevBlockHash + ringsignature.keyImage).c_str()); };
};

#endif