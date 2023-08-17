/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-network/blob/main/LICENSE for further license details.        */

#ifndef BIAS_H
#define BIAS_H

#include <string>
#include <vector>
#include <thread>
#include "structures/Node.h"
#include "structures/Blockchain.h"
#include "valency-core/custom-types/Position.h"


struct Bias {
    std::vector<OtherNodes>* otherNodes;             // The nodes on the network
    std::vector<unsigned short> previousBandwidth;   // The previous bandwidth - before bandwidth calculation was taken into account for node bias

    // Whenever the constructor is called, a thread is created and refreshBias is run constantly
    Bias(Blockchain* blockchain, std::vector<OtherNodes>* nodes, std::vector<unsigned short>* bandwidth);
    ~Bias();

    void refreshBias(Blockchain* blockchain, std::vector<OtherNodes>* nodes, std::vector<unsigned short>* bandwidth);                                   // Refresh all node biases - done by going through the blockchain and looking for staked VLNC
    void refreshBias(Blockchain* blockchain, std::string nodePublicAddress, std::vector<OtherNodes>* nodes, std::vector<unsigned short>* bandwidth);    // Refreshes a single node bias - done by going through the blockchain and looking for staked VLNC from the node public address
    
    unsigned long getBias(std::string nodePublicAddress, std::vector<OtherNodes>* nodes);    // Gets the bias of a single node
    void setBandwidth(std::vector<std::vector<unsigned short>>* nodeBandwidth);               // Changes the bias according to the input (this is run in a second thread)
};

#endif