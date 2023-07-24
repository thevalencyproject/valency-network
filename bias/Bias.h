#ifndef BIAS_H
#define BIAS_H

#include <string>
#include <vector>
#include <thread>
#include "structures/Node.h"
#include "structures/Blockchain.h"
#include "valency-core/custom-types/Position.h"


struct Bias {
    // Whenever the constructor is called, a thread is created and refreshBias is run constantly
    Bias(Blockchain* blockchain, std::vector<OtherNodes>* nodes);
    
    void refreshBias(Blockchain* blockchain, std::vector<OtherNodes>* nodes);                                   // Refresh all node biases - done by going through the blockchain and looking for staked VLNC
    void refreshBias(Blockchain* blockchain, std::string nodePublicAddress, std::vector<OtherNodes>* nodes);    // Refreshes a single node bias - done by going through the blockchain and looking for staked VLNC from the node public address
    
    unsigned long getBias(std::string nodePublicAddress, std::vector<OtherNodes>* nodes);    // Gets the bias of a single node
    void setBandwidth(std::vector<std::vector<unsigned short>>* nodeBandwidth);               // Changes the bias according to the input (this is run in a second thread)
};

#endif