/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-network/blob/main/LICENSE for further license details.        */

#ifndef SAVECONFIGURATION_H
#define SAVECONFIGURATION_H

#include <string>
#include <vector>
#include "configuration/Configuration.h"
#include "valency-core/file-recognition/file-writer/FileWriter.h"
#include "valency-core/file-recognition/file-reader/FileReader.h"


struct SaveConfiguration {
    FileWriter writer;
    FileReader reader;

    std::string path;   // The path to the configuration.vlnc file
    Configuration configuration;    // The configuration that is stored in the configuration.vlnc file

    // Set the path, read the file, decode the file into the configuration object
    SaveConfiguration() {};                     // Make sure not to call save() before reading a file!
    SaveConfiguration(std::string filepath);    // Reads the configuration on construction

    void read(std::string filepath);                                // Reads the configuration - stores configuration + filepath locally
    void read(Configuration* n, std::string filepath);    // Reads the configuration - stores configuration at pointer
    void save();                                                    // Saves the configuration to the filepath (reverse of reading the file)
    void save(Configuration* n, std::string filepath);    // Saves the configuration at the input pointer to the inputted filepath
};

#endif