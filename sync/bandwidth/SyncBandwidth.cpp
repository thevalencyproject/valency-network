#include "SyncBandwidth.h"


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
    return stoi(bandwidth);
}

std::vector<unsigned short> SyncBandwidth::convertString(std::string bandwidth) {
    std::vector<unsigned short> output;

    int prevIndex = 0;
    for(int i = 0; i < bandwidth.size(); i++) {
        if(bandwidth[i] == '.') {
            output.push_back(stoi(bandwidth.substr(prevIndex, i - prevIndex)));
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

        std::pair<std::vector<>, unsigned long> temp;
        temp.first = convertString(input);
        temp.second = nodebias;
        unverifiedbandwidth.push_back(nodebias);

        return '1';    // Return the quit code                                   
    }
}

std::string SyncBandwidth::nodeCommunicate(std::string input) {
    // Only give out details - dont send requests
    if(input[0] == '0')     // Return the bandwidth
        return '0' + convertBandwidth(bandwidth);

    if(input[0] == '1')   // Client is quitting - send the quit message
        return "/quit";
}

void SyncBandwidth::sync(std::vector<unsigned short>* bandwidth, std::vector<NodeDetails>* knownNodes) {}
void SyncBandwidth::sync(std::vector<unsigned short>* bandwidth, std::vector<NodeDetails>* knownNodes, std::vector<NodeInfo> nodes) {}
void SyncBandwidth::nodeSync(std::vector<unsigned short>* bandwidth, std::vector<NodeDetails>* knownNodes) {}
void SyncBandwidth::nodeSync(std::vector<unsigned short>* bandwidth, std::vector<NodeDetails>* knownNodes, std::vector<NodeInfo> nodes) {}