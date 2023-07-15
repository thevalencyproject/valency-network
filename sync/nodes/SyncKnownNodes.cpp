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

std::string SyncKnownNodes::communicate(std::string input) {
    if(input[0] == '0') {    // Receive the number of known nodes on the network
        input.erase(0);

        if(knownnodes.size() < stoi(input))    // If knownnodes are not up to date, send request
            return '1' + std::to_string(knownnodes.size());
        
        return "2";
    }

    if(input[0] == '1') {   // Get the known nodes returned by the individual node and store them
        input.erase(0);

        std::vector<Position3D> temp;
        temp = convertString(input);
        for(int i = 0; i < temp.size(); i++)
            knownnodes->push_back(temp[i]);

        return "2";
    }
}

std::string SyncKnownNodes::nodeCommunicate(std::string input) {
    if(input[0] == '0')   // Return the number of known nodes
        return '0' + std::to_string(knownnodes.size());

    if(input[0] == '1') {   // Return the knownnodes after the position
        input.erase(0);

        int position = stoi(input); // the index position they want nodes from

        if(position >= knownnodes.size())    // Return known nodes are already up-to-date - AKA. Quit
            return "/quit";
        
        std::vector<Position3D> temp;
        for(int i = position; i < knownnodes.size(); i++)
            temp.push_back(knownnodes[i]);

        return convertNode(&temp);
    }

    if(input[0] == '2')   // Client is quitting - send the quit message
        return "/quit";
}


void sync(std::vector<Position3D>* knownNodes, std::vector<Position3D>* activeNodes) {
    knownnodes = knownNodes;

    std::thread v(validate(activeNodes.size()));        // Validate the known nodes list

    while(1) {
        for(int i = 0; i < activeNodes.size(); i++)     // Run the client
            std::thread c(client.connectToServer(activeNodes[i].y, activeNodes[i].z, communicate, '0'));    // Initially request the # of known nodes on the network
    }
}

void sync(std::vector<Position3D>* knownNodes, std::vector<Position3D>* activeNodes, std::vector<NodeInfo> nodes) {
    knownnodes = knownNodes;

    std::thread v(validate(activeNodes.size()));        // Validate the known nodes list

    while(1) {
        for(int i = 0; i < activeNodes.size(); i++) {   // Run the onion-client
            NodeInfo n;                                 // Add the destination to the end of the nodes vector
            n.address = activeNodes[i].x;
            n.location.address = activeNodes[i].y;
            n.location.port = activeNodes[i].z
            nodes.push_back(n);

            std::thread c(onion.onionRouting(nodes, '0', communicate));     // Initially request the # of known nodes on the network

            nodes.pop_back();   // Delete the destination info
        }
    }
}

void nodeSync(std::vector<Position3D>* knownNodes, std::vector<Position3D>* activeNodes) {
    knownnodes = knownNodes;

    std::thread s(server.run(8081, nodeCommunicate));     // Run the node server
    std::thread v(validate(activeNodes.size()));          // Validate the known nodes list

    while(1) {
        for(int i = 0; i < activeNodes.size(); i++)       // Run the client
            std::thread c(client.connectToServer(activeNodes[i].y, activeNodes[i].z, communicate, '0'));    // Initially request the # of known nodes on the network
    }
}

void nodeSync(std::vector<Position3D>* knownNodes, std::vector<Position3D>* activeNodes, std::vector<NodeInfo> nodes) {
    knownnodes = knownNodes;

    std::thread s(server.run(8081, nodeCommunicate));     // Run the node server
    std::thread v(validate(activeNodes.size()));          // Validate the known nodes list

    while(1) {
        for(int i = 0; i < activeNodes.size(); i++) {     // Run the onion-client
            NodeInfo n;                                   // Add the destination to the end of the nodes vector
            n.address = activeNodes[i].x;
            n.location.address = activeNodes[i].y;
            n.location.port = activeNodes[i].z
            nodes.push_back(n);

            std::thread c(onion.onionRouting(nodes, '0', communicate));     // Initially request the # of known nodes on the network

            nodes.pop_back();   // Delete the destination info
        }
    }
}
