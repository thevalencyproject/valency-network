/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-network/blob/main/LICENSE for further license details.        */

#include "SyncBandwidth.h"


/* NEED TO DO:
    -> implemnet sentBandwidth into communication functions where 
       size (address + unsigned long bandwidth) is less than the 
       entire knownnode bandwidth vector (256 per node > 8 per node): 
       if >3.1% of nodes are active, use entire knownnodes vector 
       with unsigned long (or 8 bit equivalent) */
void SyncBandwidth::validate() {
    while(1) {
        while(unverifiedNodes.size() < numOfActiveNodes - 2)        // Wait until unverifiedNodes fills up
            std::this_thread::sleep_for(std::chrono::seconds(5));

        numOfActiveNodes = 0;   // Reset the active nodes counter

        // Verify
        std::vector<std::pair<int, unsigned long>> counter;    // first int holds index of unique bandwidth vector, whilst the int counts how many instances of it occurs
        for(int i = 0; i < unverifiedbandwidth.size(); i++) {
            // If the bandwidth vector is unseen before, add it to the vector... if it is seen, increment the instance counter
            bool seen = false;
            for(int j = 0; j < counter.size(); j++) {   // Check if it already exists in the counter
                if(unverifiedbandwidth[i].first == unverifiedbandwidth[counter[j].first].first) {
                    counter[j].second = counter[j].second + unverifiedbandwidth[i].second;    // Increment the instance counter (by the node bias)
                    seen = true;
                    break;
                }
            }
            if(seen == false) {     // Not seen, therefore add to counter vector
                std::pair<int, unsigned long> temp;
                temp.first = i;
                temp.second = unverifiedbandwidth[i].second;
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

        // Update the activebandwidth vector with the highest instance counter in the counter vector to the end of the knownnodes vector
        for(int i = 0; i < knownnodes.size(); i++) {
            knownnodes[i].bandwidth = unverifiedbandwidth[index][i];
            activebandwidth[i] = knownnodes[i].bandwidth;
        }

        unverifiedbandwidth.clear();    // Clear the vector
        counter.clear();
        highest = 0;
        index = 0;
    }
}

std::string SyncBandwidth::convertBandwidth(unsigned short* bandwidth) {
    return std::to_string(&bandwidth);
}

std::string SyncBandwidth::convertBandwidth(std::vector<unsigned short>* bandwidth) {
    std::string output;

    for(int i = 0; i < bandwidth.size(); i++)
        output = output  + std::to_string(bandwidth[i]) + '.';

    return output;
}

unsigned short SyncBandwidth::convertString(std::string bandwidth) {
    return stoul(bandwidth);
}

std::vector<unsigned short> SyncBandwidth::convertString(std::string bandwidth) {
    std::vector<unsigned short> output;

    int prevIndex = 0;
    for(int i = 0; i < bandwidth.size(); i++) {
        if(bandwidth[i] == '.') {
            output.push_back(stoul(bandwidth.substr(prevIndex, i - prevIndex)));
            prevIndex = i;
        }
    }

    return output;
}

template<typename T>
void SyncBandwidth::connectToNode(std::string* ip, int* port, std::string (T::*communicate)(std::string), std::string initialMessage) {
    if(client.connectToServer(ip, port, communicate, initialMessage) == true)
        numOfActiveNodes++;
}

template<typename T>
void SyncBandwidth::connectToNodeOnion(std::vector<NodeInfo> nodes, std::string (T::*communicate)(std::string), std::string initialMessage) {
    if(onion.connectToServer(nodes, initialMessage, communicate) == true)
        numOfActiveNodes++;
}

std::string SyncBandwidth::communicate(std::string input) {
    unsigned long nodebias = bias;
    bias = 0;

    if(input[0] == '0') {    // Get node bandwidth vector from the node
        input.erase(0);      // Erase the function index identifier

        std::pair<std::vector<unsigned long>, unsigned long> temp;
        temp.first = convertString(input);
        temp.second = nodebias;
        unverifiedbandwidth.push_back(nodebias);

        return '1';    // Return the quit code                                   
    }
}

std::string SyncBandwidth::nodeCommunicate(std::string input) {
    // Only give out details - dont send requests
    if(input[0] == '0')     // Return the bandwidth
        return '0' + convertBandwidth(&activebandwidth);

    if(input[0] == '1')   // Client is quitting - send the quit message
        return "/quit";
}

void SyncBandwidth::sync(std::vector<unsigned short>* bandwidth, std::vector<NodeDetails>* knownNodes) {
    activebandwidth = bandwidth;

    std::thread v(validate());        // Validate the bandwidth vectors

    while(1) {
        for(int i = 0; i < knownNodes.size(); i++) {     // Run the client   
            std::thread c(connectToNode(knownNodes[i].ip, 8082, communicate, '0'));     // Request the network bandwidth (bandwidth of each node)

            // Pass through the node bias
            bias = knownNodes[i].bias;    // Set the bias (for the communicate() functions)
            while(bias > 0)               // While the bias is filled (the thread hasnt gotten it yet [thread auto sets bias to zero])
                std::this_thread::sleep_for(std::chrono::milliseconds(5));   // Pause for a couple milliseconds
        }
    }
}

void SyncBandwidth::sync(std::vector<unsigned short>* bandwidth, std::vector<NodeDetails>* knownNodes, std::vector<NodeInfo> nodes) {
    activebandwidth = bandwidth;

    std::thread v(validate());        // Validate the bandwidth vectors

    while(1) {
        for(int i = 0; i < knownNodes.size(); i++) {    // Run the onion-client
            NodeInfo n;                                 // Add the destination to the end of the nodes vector
            n.address = knownNodes[i].address;
            n.location.address = knownNodes[i].ip;
            n.location.port = 8082;
            nodes.push_back(n);

            std::thread c(connectToNodeOnion(nodes, communicate, '0'));     // Initially request the network bandwidth (bandwidth of each node)

            // Pass through the node bias
            bias = knownNodes[i].bias;    // Set the bias (for the communicate() functions)
            while(bias > 0)               // While the bias is filled (the thread hasnt gotten it yet [thread auto sets bias to zero])
                std::this_thread::sleep_for(std::chrono::milliseconds(5));   // Pause for a couple milliseconds

            nodes.pop_back();   // Delete the destination info
        }
    }
}

void SyncBandwidth::nodeSync(std::vector<unsigned short>* bandwidth, std::vector<NodeDetails>* knownNodes) {
    activebandwidth = bandwidth;

    std::thread s(server.run(8082, nodeCommunicate));     // Run the node server
    std::thread v(validate());                            // Validate the block vectors

    while(1) {
        for(int i = 0; i < knownNodes.size(); i++) {    // Run the client
            std::thread c(connectToNode(knownNodes[i].ip, 8082, communicate, '0'));    // Initially request the network bandwidth (bandwidth of each node)

            // Pass through the node bias
            bias = knownNodes[i].bias;    // Set the bias (for the communicate() functions)
            while(bias > 0)               // While the bias is filled (the thread hasnt gotten it yet [thread auto sets bias to zero])
                std::this_thread::sleep_for(std::chrono::milliseconds(5);   // Pause for a couple milliseconds
        }
    }
}

void SyncBandwidth::nodeSync(std::vector<unsigned short>* bandwidth, std::vector<NodeDetails>* knownNodes, std::vector<NodeInfo> nodes) {
    activebandwidth = bandwidth;

    std::thread s(server.run(8082, nodeCommunicate));     // Run the server
    std::thread v(validate());                            // Validate the block vectors

    while(1) {
        for(int i = 0; i < knownNodes.size(); i++) {    // Run the onion-client
            NodeInfo n;                                 // Add the destination to the end of the nodes vector
            n.address = knownNodes[i].address;
            n.location.address = knownNodes[i].ip;
            n.location.port = 8082;
            nodes.push_back(n);

            std::thread c(connectToNodeOnion(nodes, communicate, '0'));     // Initially request the network bandwidth (bandwidth of each node)

            // Pass through the node bias
            bias = knownNodes[i].bias;    // Set the bias (for the communicate() functions)
            while(bias > 0)               // While the bias is filled (the thread hasnt gotten it yet [thread auto sets bias to zero])
                std::this_thread::sleep_for(std::chrono::milliseconds(5);   // Pause for a couple milliseconds

            nodes.pop_back();   // Delete the destination info
        }
    }
}