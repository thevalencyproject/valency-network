/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-network/blob/main/LICENSE for further license details.        */

#include "NodeFunctions.h"


void NodeFunctions::syncUnVerifiedBlocks() {     // Syncs any unverified blocks from other nodes

}

void NodeFunctions::syncVerifiedBlocks() {     // Syncs any valid blocks from the outgoingNodes vector (also gets valid blocks from other nodes)

}

void NodeFunctions::processIncomingBlocks() {    // Verifies any blocks in the incomingBlocks vector - loop every ~50ms (estimate): verified blocks go to outgoingBlocks vector - invalid verification go to invalidBlocks vector

}

void NodeFunctions::processInvalidBlocks() {    // See's if any invalid blocks are malicious and takes action to ban similar blocks in the future

}

void NodeFunctions::syncInvalidBlocks() {     // Syncs any invalid blocks from other nodes to filter out similar malicious blocks network wide

}

void NodeFunctions::syncKnownTransactions() {    // Syncs any known transactions details (for transactions verified by the node) - acts as references for the node

}

void NodeFunctions::processSmartContracts() {    // Processes any smart contracts stored on the network

}


void NodeFunctions::run() {
    // Run each function, each in a different thread
    while(1) {
        std::thread thread1(syncUnVerifiedBlocks());
        std::thread thread2(syncVerifiedBlocks());
        std::thread thread3(processIncomingBlocks());
        std::thread thread4(processInvalidBlocks());
        std::thread thread5(syncInvalidBlocks());
        std::thread thread6(syncKnownTransactions());
        std::thread thread7(processSmartContracts());
    }
}