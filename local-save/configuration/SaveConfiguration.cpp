#include "SaveConfiguration.h"


SaveConfiguration::SaveConfiguration(std::string filepath) {
    read(filepath);
}

void SaveConfiguration::read(std::string filepath) {
    path = filepath;
    std::vector<std::string> data = reader.getData(&path);

    configuration.maxNumOfOnionNodes = stoi(data[1]);
    configuration.minNumOfNodes = stoi(data[2]);
    configuration.minNumOfOnionNodes = stoi(data[3]);
    configuration.networkLoad = stoi(data[4]);
}

void SaveConfiguration::read(Configuration* n, std::string filepath) {
    std::vector<std::string> data = reader.getData(&filepath);

    n->maxNumOfOnionNodes = stoi(data[1]);
    n->minNumOfNodes = stoi(data[2]);
    n->minNumOfOnionNodes = stoi(data[3]);
    n->networkLoad = stoi(data[4]);
}

void SaveConfiguration::save() {
    std::vector<std::string> data;

    data.push_back(std::to_string(configuration.maxNumOfOnionNodes));
    data.push_back(std::to_string(configuration.minNumOfNodes));
    data.push_back(std::to_string(configuration.minNumOfOnionNodes));
    data.push_back(std::to_string(configuration.networkLoad));

    writer.createFile(data, &path);
}

void SaveConfiguration::save(Configuration* n, std::string filepath) {
    std::vector<std::string> data;

    data.push_back(std::to_string(n->maxNumOfOnionNodes));
    data.push_back(std::to_string(n->minNumOfNodes));
    data.push_back(std::to_string(n->minNumOfOnionNodes));
    data.push_back(std::to_string(n->networkLoad));

    writer.createFile(data, &filepath);
}