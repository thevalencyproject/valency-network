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