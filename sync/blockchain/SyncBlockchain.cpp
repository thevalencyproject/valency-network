#include "SyncBlockchain.h"


void SyncBlockchain::sync(Blockchain* blockchain, std::vector<Position3D>* activeNodes) {   // Non Onion-Routing
    chain = blockchain;

    // Run the client
    for(int i = 0; i < activeNodes.size(); i++)
        std::thread c(client.connectToServer(activeNodes[i].y, activeNodes[i].z, communicate, "0"));    // Initially request the # of shards + # of blocks in latest shard
}

void SyncBlockchain::sync(Blockchain* blockchain, std::vector<Position3D>* activeNodes, std::vector<NodeInfo> nodes) {  // Onion Routing
    chain = blockchain;

    // Run the onion-client
    for(int i = 0; i < activeNodes.size(); i++) {
        NodeInfo n;                                // Add the destination to the end of the nodes vector
        n.address = activeNodes[i].x;
        n.location.address = activeNodes[i].y;
        n.location.port = activeNodes[i].z
        nodes.push_back(n);

        std::thread c(onion.onionRouting(nodes, "0", communicate));     // Initially request the # of shards + # of blocks in latest shard

        nodes.pop_back();   // Delete the destination info
    }
}

void nodeSync(Blockchain* blockchain, std::vector<Position3D>* activeNodes) {
    chain = blockchain;
    
    std::thread s(server.run(8080, nodeCommunicate));     // Run the node server

    // Run the client
    for(int i = 0; i < activeNodes.size(); i++)
        std::thread c(client.connectToServer(activeNodes[i].y, activeNodes[i].z, communicate, "0"));    // Initially request the # of shards + # of blocks in latest shard

}

void nodeSync(Blockchain* blockchain, std::vector<Position3D>* activeNodes, std::vector<NodeInfo> nodes) {
    chain = blockchain;

    std::thread s(server.run(8080, nodeCommunicate));     // Run the server

    // Run the onion-client
    for(int i = 0; i < activeNodes.size(); i++) {
        NodeInfo n;                                // Add the destination to the end of the nodes vector
        n.address = activeNodes[i].x;
        n.location.address = activeNodes[i].y;
        n.location.port = activeNodes[i].z
        nodes.push_back(n);

        std::thread c(onion.onionRouting(nodes, "0", communicate));     // Initially request the # of shards + # of blocks in latest shard

        nodes.pop_back();   // Delete the destination info
    }
}


std::string convertBlock(Block* block) {
    // hashes are 32 characters in length
    std::string output;
    output = output + block->blockHash;        // 32 Chars
    output = output + block->prevBlockHash;    // 32 Chars
    output = output + block->time;             // 32 Chars

    output = output + block->ringsignature.keyimage;        // 32 Chars
    output = output + block->ringsignature.signature;       // 32 Chars
    output = output + block->ringsignature.receiverKey;     // 32 Chars
    output = output + block->ringsignature.transactionAmount + '.';
    output = output  + std::to_string(block->ringsignature.publicKeys.size()) + '.';
    for(int i = 0; i < block->ringsignature.publicKeys.size(); i++)
        output = output + block->ringsignature.publicKeys[i];   // Each key is 32 chars
}

std::string convertBlock(std::vector<Block>* block) {   // Can have for loops combined for optimization
    std::string output = std::to_string(block.size()) + '.';      // Append the number of blocks
    std::string temp;
    for(int i = 0; i < block.size(); i++)
        temp.push_back(convertBlock(block[i]));
        
    for(int i = 0; i < block.size(); i++)
        output = output + std::to_string(temp[i].size()) + '.';   // Append the char size of each block

    for(int i = 0; i < block.size(); i++)                         // Append the block data
        output = output + temp[i];
    
    return output;
}

Block convertString(std::string block) {
    Block output;

    output.blockHash = block.substr(0, 32);
    output.prevBlockHash = block.substr(32, 32);
    output.time = block.substr(64, 32);
    output.ringsignature.keyimage = block.substr(96, 32);
    output.ringsignature.signature = block.substr(128, 32);
    output.ringsignature.receiverKey = block.substr(160, 32);

    for(int i = 192; i < block.size(); i++) {
        if(block[i] == '.') {
            output.ringsignature.transactionAmount = stoul(block.substr(193, i - 193));
            for(int j = i + 1; j < block.size(); j++) {
                if(block[j] == '.') {
                    for(int k = j + 1; k < block.size(); k++)
                        output.ringsignature.publicKeys.push_back(block.substr(k + ((k - j) * 32), 32));

                    break;
                }
            }
            
            break;
        }
    }

    return output;
}

std::vector<Block> convertString(std::string block) {
    std::vector<Block> output;
    int numOfBlocks;
    for(int i = 0; i < block.size(); i++) {
        if(block[i] == '.') {
            numOfBlocks = stoi(block.subst(0, i - 1));    // Get the number of blocks
            block.erase(0, i);                            // Remove the numOfBlocks from the block string
            
            break;
        }
    }

    std::vector<int> blockSizes;
    for(int i = 0; i < numOfBlocks; i++) {
        for(int j = 0; j < block.size(); i++) {
            if(block[j] == '.') {
                blockSizes.push_back(stoi(block.substr(0, i - 1)));     // Get the size of the block
                block.erase(0, i);                                      // Remove the size of the block from the input string
                
                break;
            }
        }
    }

    std::string blockInput;
    for(int i = 0; i < blockSizes.size(); i++) {
        blockInput = block.substr(0, blockSizes[i]);
        output.push_back(convertString(blockInput));

        block.erase(0, blockSizes[i]);  // Remove the block from the input string
    }

}

/* We can do the following:
    -> Ask for how many shards + blocks are in the blockchain
    -> Ask for a range of shards + blocks
    -> Ask for a specific shard / block
    
    -> Receive how many shards + blocks are in the blockchain
    -> Receive the range of shards + blocks requested
    -> Receive the specific shard / block

    // Clients cannot give out data
    -> Give how many shards + blocks are in the blockchain
    -> Give the shards + blocks requested
    -> Give the specific shard / block requested

    Nodes: Act as both a server and client, acting as a server to fulfill requests, and acting as a client to send requests to other nodes.
    Clients: Act as a client, only sending requests to the nodes
*/
std::string SyncBlockchain::communicate(std::string input) {    // Wallet - STILL NEED TO DO!!!
    // Only send receive data (request is sent out in initial message)
    //  -> Initial Message should be number of shards + blocks request
    
    if(input[0] == "0") {   // Receive the shards + blocks requested
        
    }
}
std::string SyncBlockchain::nodeCommunicate(std::string input) {    // Node
    // Only give out details - dont send requests
    if(input[0] == "0")      // Return number of shards + blocks in current shard (shard being filled up)
        return "0" + std::to_string(chain->numOfShards) + "." + std::to_string(chain->chain.back().shard.size());
    
    if(input[0] == "1") {    // Return the latest block 
        return convertBlock(&chain.chain.back().shard.back());
    }

    if(input[0] == "2") {   // Return the requested block
        // Get the block position from the input string
        std::pair<int, int> blockPosition;
        for(int i = 0; i < input.size(); i++) {
            if(input[i] = '.') {
                blockPosition.first = input.substr(0, i - 1);
                input.erase(0, i);
                
                for(int j = 0; j < input.size(); j++) {
                    if(input[j] == '.') {
                        blockPosition.second = input.substr(0, j - 1);
                        input.erase(0, j);

                        break;
                    }
                }
            
                break;
            }
        }

        return convertBlock(&chain.chain[blockPositions[i].first].shard[blockPositions[i].second]);
    }

    if(input[0] == "3") {    // Return the requested blocks
        int numOfBlocks;
        std::vector<std::pair<int, int>> blockPositions;
        for(int i = 1; i < input.size(); i++) {
            if(input[i] == '.') {
                numOfBlocks = input.substr(1, i - 1);
                input.erase(0, i);

                break;
            }
        }

        for(int i = 0; i < numOfBlocks; i++) {
            std::pair<int, int> temp;

            for(int j = 0; j < input.size(); j++) {
                if(input[j] = '.') {
                    temp.first = input.substr(0, j - 1);
                    input.erase(0, j);
                    
                    for(int k = 0; k < input.size(); k++) {
                        if(input[k] == '.') {
                            temp.second = input.substr(0, k - 1);
                            input.erase(0, k);

                            break;
                        }
                    }
                
                    break;
                }
            }

            blockPositions.push_back(temp);
        }

        // Get all the blocks and add them to the block vector
        std::vector<Block> requestedBlocks;
        for(int i = 0; i < blockPositions.size(); i++)
            requestedBlocks.push_back(chain.chain[blockPositions[i].first].shard[blockPositions[i].second]);

        return convertBlock(&requestedBlocks);
    }
}


void SyncBlockchain::read(Blockchain* blockchain, std::string filePath) {
    save.read(blockchain, filePath);
}

void SyncBlockchain::save(Blockchain* blockchain, std::string filePath) {
    save.save(blockchain, filePath);
}