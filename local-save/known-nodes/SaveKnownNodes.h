#ifndef SAVEKNOWNNODES_H
#define SAVEKNOWNNODES_H

#include "valency-core/custom-types/Position.h"
#include "valency-core/file-recognition/file-writer/FileWriter.h"
#include "valency-core/file-recognition/file-reader/FileReader.h"


struct SaveKnownNodes {
    FileWriter writer;
    FileReader reader;

    std::string path;                       // The path to the known-nodes.vlnc file
    std::vector<Position3D> knownNodes;     // The known nodes vector that is stored in the known-nodes.vlnc file

    // Set the path, read the file, decode the file into the known nodes vector
    SaveKnownNodes(std::string filepath);   // Reads the known nodes
    void save();                            // Saves the known nodes to the filepath (reverse of reading the file)
};

#endif