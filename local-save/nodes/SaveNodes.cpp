/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-network/blob/main/LICENSE for further license details.        */

#include "SaveNodes.h"


SaveNodes::SaveNodes(std::string filepath) {
    read(filepath);
}

void SaveNodes::read(std::string filepath) {
    path = filepath;
    std::vector<std::string> data = reader.getData(&path);

    NodeDetails temp;
    for(int i = 0; i < data.size() / 4; i++) {
        temp.address = data[i * 3];                // Node Public Address
        temp.ip = data[(i * 3) + 1];               // Node IP Address
        temp.port = stoi(data[(i * 3) + 2]);       // Node Port
        temp.bias = stoull(data[(i * 3) + 3]);     // Node Bias
        nodes.push_back(temp);
    }
}

void SaveNodes::read(std::vector<NodeDetails>* n, std::string filepath) {
    std::vector<std::string> data = reader.getData(&filepath);

    NodeDetails temp;
    for(int i = 0; i < data.size() / 4; i++) {
        temp.address = data[i * 3];                // Node Public Address
        temp.ip = data[(i * 3) + 1];               // Node IP Address
        temp.port = stoi(data[(i * 3) + 2]);       // Node Port
        temp.bias = stoull(data[(i * 3) + 3]);     // Node Bias
        n.push_back(temp);
    }
}

void SaveNodes::save() {
    std::vector<std::string> data;

    for(int i = 0; i < nodes.size(); i++) {
        data.push_back(nodes[i].address);
        data.push_back(nodes[i].ip);
        data.push_back(std::to_string(nodes[i].port));
        data.push_back(std::to_string(nodes[i].bias));
    }

    writer.createFile(data, &path);
}

void SaveNodes::save(std::vector<NodeDetails>* n, std::string filepath) {
    std::vector<std::string> data;

    for(int i = 0; i < n.size(); i++) {
        data.push_back(n[i].address);
        data.push_back(n[i].ip);
        data.push_back(std::to_string(n[i].port));
        data.push_back(std::to_string(n[i].bias));
    }

    writer.createFile(data, &filepath);
}