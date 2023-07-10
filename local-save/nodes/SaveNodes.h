#ifndef SAVENODES_H
#define SAVENODES_H

#include "valency-core/custom-types/Position.h"
#include "valency-core/file-recognition/file-writer/FileWriter.h"
#include "valency-core/file-recognition/file-reader/FileReader.h"


struct SaveNodes {
    FileWriter writer;
    FileReader reader;

    std::string path;                  // The path to the nodes.vlnc file
    std::vector<Position3D> nodes;     // The nodes vector that is stored in the nodes.vlnc file

    // Set the path, read the file, decode the file into the nodes vector
    SaveNodes() {};                     // Make sure not to call save() before reading a file!
    SaveNodes(std::string filepath);    // Reads the nodes on construction

    void read(std::string filepath);    // Reads the nodes
    void save();                        // Saves the nodes to the filepath (reverse of reading the file)
};

#endif