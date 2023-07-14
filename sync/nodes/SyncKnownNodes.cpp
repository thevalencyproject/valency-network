#include "SyncKnownNodes.h"


std::string SyncKnownNodes::convertNode(Position3D* node) {
// ':' between IP address and port + 32 character address at front

    return node.x + node.y + '.' + std::to_string(node.z);
}

std::string SyncKnownNodes::convertNode(std::vector<Position3D>* nodes) {
    std::string output = std::to_string(nodes.size()) + '.';    // Append the number of nodes


    for(int i = 0; i < nodes.size(); i++)
        output = output + node.x + node.y + ':' + std::to_string(node.z) + '.';     // '.' between nodes

    output.pop_back();  // Remove last '.'
    return output;
}

Position3D SyncKnownNodes::convertString(std::string node) {
    Position3D output;
    
    output.x = node.substr(0, 32);
    for(int i = 33; i < node.size(); i++) {
        if(node[i] == ':') {
            output.y = node.substr(33, i - 1 - 33);
            output.z = node.substr(i - 1, node.size() - i - 1);
            break;
        }
    }

    return output;
}

std::vector<Position3D> SyncKnownNodes::convertString(std::string nodes) {
    std::vector<Position3D> output;

    // Get the number of nodes
    int size;
    for(int i = 0; i < nodes.size(); i++) {
        if(nodes[i] == '.') {
            size = stoi(nodes.substr(0, i - 1));
            nodes.erase(0, i);
            break;
        }
    }

    for(int i = 0; i < size; i++) {     // Loop once for each node in the string
        Position3D temp;

        temp.x = nodes.substr(0, 32);   // Get the public node address
        for(int j = 33; j < nodes.size(); j++) {    // Get the IP address
            if(nodes[i] == ':') {
                temp.y = nodes.substr(33, j - 1 - 33);
                
                for(int k = j; k < node.size(); k++) {    // Get the Port
                    if(nodes[j] == '.') {
                        temp.z = nodes.substr(j, k - j);
                        nodes.erase(0, j);

                        break;
                    }
                }

                break;
            }
        }

        output.push_back(temp);
    }

    return output;
}