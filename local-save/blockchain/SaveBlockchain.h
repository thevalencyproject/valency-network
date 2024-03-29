/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-network/blob/main/LICENSE for further license details.        */

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
    SaveBlockchain() {};                    // Make sure not to call save() before reading a file!
    SaveBlockchain(std::string filepath);   // Reads the blockchain on construction

    void read(std::string filepath);                       // Reads the blockchain
    void read(Blockchain* chain, std::string filepath);    // Reads the blockchain - stores blockchain at pointer
    void save();                                           // Saves the blockchain to the filepath (reverse of reading the file)
    void save(Blockchain* chain, std::string filepath);    // Saves the blockchain at the input pointer to the inputted filepath
};

#endif