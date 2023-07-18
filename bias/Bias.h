#ifndef BIAS_H
#define BIAS_H

#include <string>
#include <vector>
#include "structures/Blockchain.h"
#include "valency-core/custom-types/Position.h"


struct Bias {
    Bias() {};

    void refreshBias(Blockchain* blockchain, std::vector<Position4D>* nodes);                                   // Refresh all node biases - done by going through the blockchain and looking for staked VLNC
    void refreshBias(Blockchain* blockchain, std::string nodePublicAddress, std::vector<Position4D>* nodes);    // Refreshes a single node bias - done by going through the blockchain and looking for staked VLNC from the node public address
    
    unsigned long getBias(std::string nodePublicAddress, std::vector<Position4D>* nodes);    // Gets the bias of a single node
};

#endif