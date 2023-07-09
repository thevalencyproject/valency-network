#include "SaveKnownNodes.h"


SaveKnownNodes::SaveKnownNodes(std::string filepath) {
    path = filepath;
    std::vector<std::string> data = reader.getData(&path);

    Position3D temp;
    for(int i = 0; i < data.size() / 3; i++) {
        temp.x = data[i * 3];                 // Node Public Address
        temp.y = data[(i * 3) + 1];           // Node IP Address
        temp.z = stoi(data[(i * 3) + 2]);     // Node Port
        knownNodes.push_back(temp);
    }
}

void SaveKnownNodes::save() {
    std::vector<std::string> data;

    for(int i = 0; i < knownNodes.size(); i++) {
        data.push_back(knownNodes[i].x);
        data.push_back(knownNodes[i].y);
        data.push_back(std::to_string(knownNodes[i].z));
    }

    writer.createFile(data, &path);
}