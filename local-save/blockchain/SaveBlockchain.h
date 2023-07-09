#ifndef SAVEBLOCKCHAIN_H
#define SAVEBLOCKCHAIN_H

#include "structures/Blockchain.h"
#include "valency-core/file-recognition/file-writer/FileWriter.h"
#include "valency-core/file-recognition/file-reader/FileReader.h"


struct SaveBlockchain {
    FileWriter writer;
    FileReader reader;

    std::string path;           // The path to the blockchain.vlnc file
    Blockchain blockchain;      // The blockchain that is stored in the blockchain.vlnc file

    // Set the path, read the file, decode the file into the blockchain
    /* First 2 lines of data are ID, then length...
       Next lines are the chain (starting from zero to chain.size()):
          -> Shard Hash
          -> Prev Shard Hash
          -> Next are the shard blocks (multiple starting from zero to shard.size())
                -> Block Hash
                -> Previous Block Hash
                -> Time */
    SaveBlockchain(std::string filepath);   // Reads the blockchain
    void save();                            // Saves the blockchain to the filepath (reverse of reading the file)
};

#endif