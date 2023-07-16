#include "SyncBlockchain.h"


void SyncBlockchain::validate(int numOfActiveNodes) {
    while(1) {
        while(unVerifiedBlocks.size() < numOfActiveNodes - 2)      // Wait for until unVerifiedBlocks fills up
            std::this_thread::sleep_for(std::chrono::seconds(5));

        // Verify
        std::vector<std::pair<int, int>> counter;    // first int holds index of unique block vector, whilst the int counts how many instances of it occurs
        for(int i = 0; i < unVerifiedBlocks.size(); i++) {
            // If the block vector is unseen before, add it to the vector of pairs... if it is seen, increment the instance counter
            bool seen = false;
            for(int j = 0; j < counter.size(); j++) {   // Check if it already exists in the counter
                if(unverifiedBlocks[i] == unverifiedBlocks[counter[j].first]) {
                    counter[j].second++;    // Increment the instance counter
                    seen = true;
                    break;
                }
            }
            if(seen == false) {     // Not seen, therefore add to counter vector
                std::pair<int, int> temp;
                temp.first = i;
                temp.second = 1;
            }
        }

        // Sort the counter vector by the highest number
        int highest, index;
        for(int i = 0; i < counter.size(); i++) {
            if(counter[i].second > highest) {
                highest = counter[i].second;
                index = counter[i].first;
            }
        }

        // Set the blockchain to the block vector with the highest instance counter in the counter vector
        for(int i = 0; i < unverifiedBlocks[index].size(); i++)
            chain->addBlock(unverifiedBlocks[index][i]);

        unVerifiedBlocks.clear();   // Clear the vector
        counter.clear();
        highest = 0;
        index = 0;
    }
}

void SyncBlockchain::sync(Blockchain* blockchain, std::vector<Position3D>* activeNodes) {   // Non Onion-Routing
    chain = blockchain;

    std::thread v(validate(activeNodes.size()));        // Validate the block vectors

    while(1) {
        for(int i = 0; i < activeNodes.size(); i++)     // Run the client
            std::thread c(client.connectToServer(activeNodes[i].y, activeNodes[i].z, communicate, '0'));    // Initially request the # of shards + # of blocks in latest shard
    }
}

void SyncBlockchain::sync(Blockchain* blockchain, std::vector<Position3D>* activeNodes, std::vector<NodeInfo> nodes) {  // Onion Routing
    chain = blockchain;

    std::thread v(validate(activeNodes.size()));        // Validate the block vectors

    while(1) {
        for(int i = 0; i < activeNodes.size(); i++) {   // Run the onion-client
            NodeInfo n;                                 // Add the destination to the end of the nodes vector
            n.address = activeNodes[i].x;
            n.location.address = activeNodes[i].y;
            n.location.port = activeNodes[i].z
            nodes.push_back(n);

            std::thread c(onion.onionRouting(nodes, '0', communicate));     // Initially request the # of shards + # of blocks in latest shard

            nodes.pop_back();   // Delete the destination info
        }
    }
}

void SyncBlockchain::nodeSync(Blockchain* blockchain, std::vector<Position3D>* activeNodes) {
    chain = blockchain;
    
    std::thread s(server.run(8080, nodeCommunicate));     // Run the node server
    std::thread v(validate(activeNodes.size()));          // Validate the block vectors

    while(1) {
        for(int i = 0; i < activeNodes.size(); i++)     // Run the client
            std::thread c(client.connectToServer(activeNodes[i].y, activeNodes[i].z, communicate, '0'));    // Initially request the # of shards + # of blocks in latest shard
    }
}

void SyncBlockchain::nodeSync(Blockchain* blockchain, std::vector<Position3D>* activeNodes, std::vector<NodeInfo> nodes) {
    chain = blockchain;

    std::thread s(server.run(8080, nodeCommunicate));     // Run the server
    std::thread v(validate(activeNodes.size()));          // Validate the block vectors

    while(1) {
        for(int i = 0; i < activeNodes.size(); i++) {    // Run the onion-client
            NodeInfo n;                                  // Add the destination to the end of the nodes vector
            n.address = activeNodes[i].x;
            n.location.address = activeNodes[i].y;
            n.location.port = activeNodes[i].z
            nodes.push_back(n);

            std::thread c(onion.onionRouting(nodes, '0', communicate));     // Initially request the # of shards + # of blocks in latest shard

            nodes.pop_back();   // Delete the destination info
        }
    }
}


std::string SyncBlockchain::convertBlock(Block* block) {
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

std::string SyncBlockchain::convertBlock(std::vector<Block>* block) {   // Can have for loops combined for optimization
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

Block SyncBlockchain::convertString(std::string block) {
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

std::vector<Block> SyncBlockchain::convertString(std::string block) {
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

std::string SyncBlockchain::communicate(std::string input) {    // Wallet
    // Only send receive data (request is sent out in initial message)
    //  -> Initial Message should be number of shards + blocks request

    if(input[0] == '0') {   // Receive the shards + blocks requested
        input.erase(0);     // Erase the function index identifier

        // Loop until a '.' is found, Left = Shard Num, Right = Block Num
        std::pair<int, int> position;   // first = shardnum, second = blocknum
        for(int i = 0; i < input.size(); i++) {
            if(input[i] == '.') {
                position.first = stoi(input.substr(0, i - 1));
                position.second = stoi(input.substr(i, input.size() - i - 1));

                break;
            }
        }

        // Check result against things - if things dont match, ask for blocks required
        std::pair<int, int> localPos;
        localPos.first = chain->numOfShards;
        localPos.second = chain->chain.back().shard.size();
        if(localPos.first != position.first || localPos.second != position.second) {
            // Send the localPosition to the node - this will send back the required blocks to get updated
            return '1' + std::to_string(localPos.first) + '.' + std::to_string(localPos.second);
        }

        return '2';     // Blockchain is up-to-date - end connection
    }

    if(input[0] == '1') {                                   // Get the blocks returned by the individual node and store them
        input.erase(0);                                     // Erase the function index identifier
        unverifiedBlocks.push_back(convertString(input));   // Add the unverified blocks to the vector
        return '2';                                         // Return the quit code                                   
    }
}
std::string SyncBlockchain::nodeCommunicate(std::string input) {    // Node
    // Only give out details - dont send requests
    if(input[0] == '0')     // Return number of shards + blocks in current shard (shard being filled up)
        return '0' + std::to_string(chain->numOfShards) + '.' + std::to_string(chain->chain.back().shard.size());
    
    if(input[0] == '1') {   // Return the blocks after the position
        input.erase(0);     // Erase the function index identifier

        // Loop until a '.' is found, Left = Shard Num, Right = Block Num
        std::pair<int, int> position;   // first = shardnum, second = blocknum
        for(int i = 0; i < input.size(); i++) {
            if(input[i] == '.') {
                position.first = stoi(input.substr(0, i - 1));
                position.second = stoi(input.substr(i, input.size() - i - 1));

                break;
            }
        }

        if(position.first == chain->numOfShards && position.second == chain->chain.back().shard.size())
            return "/quit";     // Return blockchain already up to date - AKA. Quit
        
        // Fill the block vector
        std::vector<Block> requestedBlocks;

        // FIRST: Fill semi-filled shard: ShardPos = position.first, BlockPos = i
        for(int i = position.second; i < chain->length; i++)
            requestedBlocks.push_back(chain->chain[position.first].shard[i]);

        // SECOND: Fill non-filled shards: ShardPos = i, BlockPos = j
        for(int i = position.first + 1; i < chain->numOfShards - 1; i++)
            for(int j = 0; j < chain->length; j++)
                requestedBlocks.push_back(chain->chain[i].shard[j]);

        // THIRD: Fill final shard (up to block amount): ShardPos = chain->chain.size(), BlockPos = i
        for(int i = 0; i < chain->chain.back().shard.size(); i++)
            requestedBlocks.push_back(chain->chain[chain->chain.size()].shard[i]);

        return '1' + convertBlock(&requestedBlocks);
    }

    if(input[0] == '2')   // Client is quitting - send the quit message
        return "/quit";
}


void SyncBlockchain::read(Blockchain* blockchain, std::string filePath) {
    save.read(blockchain, filePath);
}

void SyncBlockchain::save(Blockchain* blockchain, std::string filePath) {
    save.save(blockchain, filePath);
}