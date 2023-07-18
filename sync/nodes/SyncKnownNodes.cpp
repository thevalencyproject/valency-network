#include "SyncKnownNodes.h"


void SyncKnownNodes::validate() {
    while(1) {
        while(unverifiedNodes.size() < numOfknownnodes - 2)        // Wait until unverifiedNodes fills up
            std::this_thread::sleep_for(std::chrono::seconds(5));

        numOfknownnodes = 0;   // Reset the active nodes counter

        // Verify
        std::vector<std::pair<int, unsigned long>> counter;    // first int holds index of unique node vector, whilst the int counts how many instances of it occurs
        for(int i = 0; i < unverifiedNodes.size(); i++) {
            // If the node vector is unseen before, add it to the vector... if it is seen, invrement the instance counter
            bool seen = false;
            for(int j = 0; j < counter.size(); j++) {   // Check if it already exists in the counter
                if(unverifiedNodes[i].first == unverifiedNodes[counter[j].first].first) {
                    counter[j].second = counter[j].second + unverifiedNodes[i].second;    // Increment the instance counter (by the node bias)
                    seen = true;
                    break;
                }
            }
            if(seen == false) {     // Not seen, therefore add to counter vector
                std::pair<int, unsigned long> temp;
                temp.first = i;
                temp.second = unverifiedNodes[i].second;
                counter.push_back(temp);
            }
        }

        // Sort the counter vector to find the highest number
        unsigned long highest, index;
        for(int i = 0; i < counter.size(); i++) {
            if(counter[i].second > highest) {
                highest = counter[i].second;
                index = counter[i].first;
            }
        }

        // Add the knownnodes temp vector with the highest instance counter in the counter vector to the end of the known nodes vector
        for(int i = 0; i < unverifiedNodes[index].size(); i++)
            knownnodes->push_back(unverifiedNodes[index][i]);
        
        unverifiedNodes.clear();    // Clear the vector
        counter.clear();
        highest = 0;
        index = 0;
    }
}

std::string SyncKnownNodes::convertNode(Position4D* node) {
// ':' between IP address and port + 32 character address at front

    return node.x + node.y + '.' + std::to_string(node.z) + '.' + std::to_string(node.i);
}

std::string SyncKnownNodes::convertNode(std::vector<Position4D>* nodes) {
    std::string output = std::to_string(nodes.size()) + '.';    // Append the number of nodes


    for(int i = 0; i < nodes.size(); i++)
        output = output + node.x + node.y + ':' + std::to_string(node.z) + '.' + std::to_string(node.i) + '.';     // '.' between nodes

    output.pop_back();  // Remove last '.'
    return output;
}

Position4D SyncKnownNodes::convertString(std::string node) {
    Position4D output;
    
    output.x = node.substr(0, 32);
    for(int i = 33; i < node.size(); i++) {
        if(node[i] == ':') {
            output.y = node.substr(33, i - 1 - 33);

            for(int j = i; j < node.size(); j++) {
                if(node[j] == '.') {
                    output.z = node.substr(i, j - i);
                    output.i = node.substr(j, node.size() - j);

                    break;
                }
            }

            break;
        }
    }

    return output;
}

std::vector<Position4D> SyncKnownNodes::convertString(std::string nodes) {
    std::vector<Position4D> output;

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
        Position4D temp;

        temp.x = nodes.substr(0, 32);   // Get the public node address
        for(int j = 33; j < nodes.size(); j++) {    // Get the IP address
            if(nodes[j] == ':') {
                temp.y = nodes.substr(33, j - 1 - 33);
                
                for(int k = j; k < nodes.size(); k++) {    // Get the Port
                    if(nodes[k] == '.') {
                        temp.z = nodes.substr(j, k - j);

                        for(int x = k; x < nodes.size(); x++) {
                            if(nodes[x] == '.') {
                                temp.i = nodes.substr(k, x - k)
                                nodes.erase(0, x);

                                break;
                            }
                        }

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

template<typename T>
void SyncKnownNodes::connectToNode(std::string* ip, int* port, std::string (T::*communicate)(std::string), std::string initialMessage) {
    if(client.connectToServer(ip, port, communicate, initialMessage) == true)
        numOfknownnodes++;
}

template<typename T>
void SyncKnownNodes::connectToNodeOnion(std::vector<NodeInfo> nodes, std::string (T::*communicate)(std::string), std::string initialMessage) {
    if(onion.connectToServer(nodes, initialMessage, communicate) == true)
        numOfknownnodes++;
}

std::string SyncKnownNodes::communicate(std::string input) {
    unsigned long nodebias = bias;
    bias = 0;

    if(input[0] == '0') {    // Receive the number of known nodes on the network
        input.erase(0);

        if(knownnodes.size() < stoi(input))    // If knownnodes are not up to date, send request
            return '1' + std::to_string(knownnodes.size());
        
        return "2";
    }

    if(input[0] == '1') {   // Get the known nodes returned by the individual node and store them
        input.erase(0);

        // Add the unverified known nodes to the vector (Position3D

std::string SyncKnownNodes::nodeCommunicate(std::string input) {
    if(input[0] == '0')   // Return the number of known nodes
        return '0' + std::to_string(knownnodes.size());

    if(input[0] == '1') {   // Return the knownnodes after the position
        input.erase(0);

        int position = stoi(input); // the index position they want nodes from

        if(position >= knownnodes.size())    // Return known nodes are already up-to-date - AKA. Quit
            return "/quit";
        
        std::vector<Position4D> temp;
        for(int i = position; i < knownnodes.size(); i++)
            temp.push_back(knownnodes[i]);

        return convertNode(&temp);
    }

    if(input[0] == '2')   // Client is quitting - send the quit message
        return "/quit";
}


void SyncKnownNodes::sync(std::vector<Position4D>* knownNodes) {
    knownnodes = knownNodes;

    std::thread v(validate(knownnodes.size()));        // Validate the known nodes list

    while(1) {
        for(int i = 0; i < knownnodes.size(); i++) {     // Run the client
            std::thread c(connectToNode(knownnodes[i].y, knownnodes[i].z, communicate, '0'));    // Initially request the # of known nodes on the network
        
            // Pass through the node bias
            bias = knownnodes[i].i;    // Set the bias (for the communicate() functions)
            while(bias > 0)             // While the bias is filled (the thread hasn't gotten it yet [thread auto sets bias to zero])
                std::this_thread::sleep_for(std::chrono::milliseconds(5));  // Pause for a couple milliseconds
        }
    }
}

void SyncKnownNodes::sync(std::vector<Position4D>* knownNodes, std::vector<NodeInfo> nodes) {
    knownnodes = knownNodes;

    std::thread v(validate(knownnodes.size()));        // Validate the known nodes list

    while(1) {
        for(int i = 0; i < knownnodes.size(); i++) {   // Run the onion-client
            NodeInfo n;                                 // Add the destination to the end of the nodes vector
            n.address = knownnodes[i].x;
            n.location.address = knownnodes[i].y;
            n.location.port = knownnodes[i].z
            nodes.push_back(n);

            std::thread c(connectToNodeOnion(nodes, communicate, '0'));     // Initially request the # of known nodes on the network

            // Pass through the node bias
            bias = knownnodes[i].i;    // Set the bias (for the communicate() functions)
            while(bias > 0)             // While the bias is filled (the thread hasn't gotten it yet [thread auto sets bias to zero])
                std::this_thread::sleep_for(std::chrono::milliseconds(5));  // Pause for a couple milliseconds

            nodes.pop_back();   // Delete the destination info
        }
    }
}

void SyncKnownNodes::nodeSync(std::vector<Position4D>* knownNodes) {
    knownnodes = knownNodes;

    std::thread s(server.run(8081, nodeCommunicate));     // Run the node server
    std::thread v(validate(knownnodes.size()));          // Validate the known nodes list

    while(1) {
        for(int i = 0; i < knownnodes.size(); i++) {      // Run the client
            std::thread c(connectToNode(knownnodes[i].y, knownnodes[i].z, communicate, '0'));    // Initially request the # of known nodes on the network

            // Pass through the node bias
            bias = knownnodes[i].i;    // Set the bias (for the communicate() functions)
            while(bias > 0)             // While the bias is filled (the thread hasn't gotten it yet [thread auto sets bias to zero])
                std::this_thread::sleep_for(std::chrono::milliseconds(5));  // Pause for a couple milliseconds
        }
    }
}

void SyncKnownNodes::nodeSync(std::vector<Position4D>* knownNodes, std::vector<NodeInfo> nodes) {
    knownnodes = knownNodes;

    std::thread s(server.run(8081, nodeCommunicate));     // Run the node server
    std::thread v(validate(knownnodes.size()));          // Validate the known nodes list

    while(1) {
        for(int i = 0; i < knownnodes.size(); i++) {     // Run the onion-client
            NodeInfo n;                                   // Add the destination to the end of the nodes vector
            n.address = knownnodes[i].x;
            n.location.address = knownnodes[i].y;
            n.location.port = knownnodes[i].z
            nodes.push_back(n);

            std::thread c(connectToNodeOnion(nodes, communicate, '0'));     // Initially request the # of known nodes on the network

            // Pass through the node bias
            bias = knownnodes[i].i;    // Set the bias (for the communicate() functions)
            while(bias > 0)             // While the bias is filled (the thread hasn't gotten it yet [thread auto sets bias to zero])
                std::this_thread::sleep_for(std::chrono::milliseconds(5));  // Pause for a couple milliseconds

            nodes.pop_back();   // Delete the destination info
        }
    }
}

void SyncKnownNodes::read(std::vector<Position4D>* knownNodes, std::string filePath) {
    save.read(knownNodes, filePath);
}

void SyncKnownNodes::save(std::vector<Position4D>* knownNodes, std::string filePath) {
    save.save(knownNodes, filePath);
}