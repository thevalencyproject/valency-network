/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-network/blob/main/LICENSE for further license details.        */

#ifndef SAVENODES_H
#define SAVENODES_H

#include "/structures/Node.h"
#include "valency-core/custom-types/Position.h"
#include "valency-core/file-recognition/file-writer/FileWriter.h"
#include "valency-core/file-recognition/file-reader/FileReader.h"


struct SaveNodes {
    FileWriter writer;
    FileReader reader;

    std::string path;                   // The path to the nodes.vlnc file
    std::vector<NodeDetails> nodes;     // The nodes vector that is stored in the nodes.vlnc file

    // Set the path, read the file, decode the file into the nodes vector
    SaveNodes() {};                     // Make sure not to call save() before reading a file!
    SaveNodes(std::string filepath);    // Reads the nodes on construction

    void read(std::string filepath);                                 // Reads the nodes - stores nodes + filepath locally
    void read(std::vector<NodeDetails>* n, std::string filepath);    // Reads the nodes - stores nodes at pointer
    void save();                                                     // Saves the nodes to the filepath (reverse of reading the file)
    void save(std::vector<NodeDetails>* n, std::string filepath);    // Saves the nodes at the input pointer to the inputted filepath
};

#endif