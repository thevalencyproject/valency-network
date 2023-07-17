#include "SaveNodes.h"


/* x: Public Address
    y: IP Address
    z: Port
    i: Bias */
SaveNodes::SaveNodes(std::string filepath) {
    read(filepath);
}

void SaveNodes::read(std::string filepath) {
    path = filepath;
    std::vector<std::string> data = reader.getData(&path);

    Position4D temp;
    for(int i = 0; i < data.size() / 4; i++) {
        temp.x = data[i * 3];                   // Node Public Address
        temp.y = data[(i * 3) + 1];             // Node IP Address
        temp.z = stoi(data[(i * 3) + 2]);       // Node Port
        temp.i = stoull(data[(i * 3) + 3]);    // Node Bias
        nodes.push_back(temp);
    }
}

void SaveNodes::read(std::vector<Position4D>* n, std::string filepath) {
    std::vector<std::string> data = reader.getData(&filepath);

    Position4D temp;
    for(int i = 0; i < data.size() / 4; i++) {
        temp.x = data[i * 3];                   // Node Public Address
        temp.y = data[(i * 3) + 1];             // Node IP Address
        temp.z = stoi(data[(i * 3) + 2]);       // Node Port
        temp.i = stoull(data[(i * 3) + 3]);    // Node Bias
        n.push_back(temp);
    }
}

void SaveNodes::save() {
    std::vector<std::string> data;

    for(int i = 0; i < nodes.size(); i++) {
        data.push_back(nodes[i].x);
        data.push_back(nodes[i].y);
        data.push_back(std::to_string(nodes[i].z));
        data.push_back(std::to_string(nodes[i].i));
    }

    writer.createFile(data, &path);
}

void SaveNodes::save(std::vector<Position4D>* n, std::string filepath) {
    std::vector<std::string> data;

    for(int i = 0; i < n.size(); i++) {
        data.push_back(n[i].x);
        data.push_back(n[i].y);
        data.push_back(std::to_string(n[i].z));
        data.push_back(std::to_string(n[i].i));
    }

    writer.createFile(data, &filepath);
}