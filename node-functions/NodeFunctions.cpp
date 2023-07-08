#include "NodeFunctions.h"


void NodeFunctions::syncBlockchain() {    // Syncs the Blockchain up with all other nodes on the network

}

void NodeFunctions::syncActiveNodes() {     // Syncs any active nodes (pushes address + IP + port into activeNodes vector) - also adds them to knownNodes vector + knownNodes file if they are unknown

}

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


void NodeFunctions::run() {+
    while(1) {
        // Run each function, each in a different thread
        std::thread thread1(syncBlockchain());
        std::thread thread2(syncActiveNodes());
        std::thread thread3(syncUnVerifiedBlocks());
        std::thread thread4(syncVerifiedBlocks());
        std::thread thread5(processIncomingBlocks());
        std::thread thread6(processInvalidBlocks());
        std::thread thread7(syncInvalidBlocks());
        std::thread thread8(syncKnownTransactions());
        std::thread thread9(processSmartContracts());
    }
}