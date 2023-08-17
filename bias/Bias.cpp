/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-network/blob/main/LICENSE for further license details.        */

#include "Bias.h"


Bias::Bias(Blockchain* blockchain, std::vector<OtherNodes>* nodes, std::vector<unsigned short>* bandwidth) {
    previousBandwidth = &bandwidth;     // Set the previous bandwidth vector - nodebias takes away bandwidth on destruction
    otherNodes = nodes;

    while(1)
        std::thread bias(refreshBias(blockchain, nodes, bandwidth));   // Constantly refresh biases
}
Bias::~Bias() {                         // Remove the bandwidth bias (calculated locally)
    for(int i = 0; i < otherNodes.size(); i++)
        otherNodes[i].bias = otherNodes[i].bias - previousBandwidth[i];
}


// So far, this bias function runs 100% on proof-of-stake, however, bias will be added based on node bandwidth (this will be dynamic like the whitepaper states)
void Bias::refreshBias(Blockchain* blockchain, std::vector<OtherNodes>* nodes, std::vector<unsigned short>* bandwidth) {


    for(int i = 0; i < blockchain->numOfShards; i++) {   // Loop through each shard in the blockchain
        for(int j = 0; j < blockchain->length; j++) {    // Loop through each block in each shard

            // Staking VLNC involves not implementing any stealth addresses (addresses are public) - the signature ring should consist of the receiver address and the sender address
            //      -> Receiver Address is the first index, Sender Address is the second index in the publicKeys ring!
            if(blockchain->chain[i].shard[j].signature.publicKeys.size() == 2) {
                // Search that block to see if a node has sent VLNC to stake
                for(int k = 0; k < nodes.size(); k++) {
                    // If the transaction is sending VLNC to the node address, that is adding to the staked amount (bias) for the node
                    if(blockchain->chain[i].shard[j].signature.publicKeys[0] == nodes[k].details.address) {
                        // Add transaction amount to bias
                        nodes[k].details.bias = nodes[k].details.bias + blockchain->chain[i].shard[j].signature.data.transactionAmount;
                    }

                    // If the transaction is sending VLNC from the node address, that is subtracting from the staked amount (bias) for the node
                    if(blockchain->chain[i].shard[j].signature.publicKeys[1] == nodes[k].x) {      
                        // Subtract transaction amount from bias
                        nodes[k].details.bias = nodes[k].details.bias - blockchain->chain[i].shard[j].signature.data.transactionAmount;
                    }

                    // Update bandwidth by adding bandwidth scores to node biases (after subtracting from prevBandwidth values)
                    for(int i = 0; i < nodes.size(); i++) {
                        nodes[i].bias = nodes[i].bias - previousBandwidth[i];
                        nodes[i].bias = nodes[i].bias + bandwidth[i];
                    }
                    previousBandwidth = &bandwidth;
                }
            }
        }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(5));  // Pause for 5ms
}

void Bias::refreshBias(Blockchain* blockchain, std::string nodePublicAddress, std::vector<unsigned short>* bandwidth, std::vector<OtherNodes>* nodes) {
    unsigned int bias;
    for(int i = 0; i < blockchain->numOfShards; i++) {   // Loop through each shard in the blockchain
        for(int j = 0; j < blockchain->length; j++) {    // Loop through each block in each shard
            
            if(blockchain->chain[i].shard[j].signature.publicKeys.size() == 2) {
                if(blockchain->chain[i].shard[j].signature.publicKeys[0] == nodePublicAddress) {
                    // Add transaction amount to bias
                    bias = bias + blockchain->chain[i].shard[j].signature.data.transactionAmount;
                }

                // If the transaction is sending VLNC from the node address, that is subtracting from the staked amount (bias) for the node
                if(blockchain->chain[i].shard[j].signature.publicKeys[1] == nodePublicAddress) {      
                    // Subtract transaction amount from bias
                    bias = bias - blockchain->chain[i].shard[j].signature.data.transactionAmount;
                }
            }
        }
    }

    // Get the bias of the node and find add bias to it
    for(int i = 0; i < nodes.size(); i++)
        if(nodePublicAddress == nodes[i].details.address)
            nodes[i].details.bias = nodes[i].details.bias + bias;
}

unsigned long Bias::getBias(std::string nodePublicAddress, std::vector<OtherNodes> nodes) {
    for(int i = 0; i < nodes.size(); i++)
        if(nodePublicAddress == nodes[i].details.address)
            return nodes[i].details.bias;
}