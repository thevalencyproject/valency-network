#include "SaveBlockchain.h"


SaveBlockchain::SyncBlockchain(std::string filepath) {
    read(filepath);
}

void SaveBlockchain::read(std::string filepath) {
    path = filepath;
    std::vector<std::string> data = reader.getData(&path);

    blockchain.id = data[0];                   // The blockchain unique identifier
    blockchain.length = stoul(data[1]);        // The shard length
    blockchain.numOfShards = stoul(data[2]);   // The number of shards in the chain

    // numOfShards - 1 to negate the last shard (last shard may not be full)
    for(int i = 1; i <= (blockchain.numOfShards - 1); i++) {
        unsigned int index = (blockchain.length * 3 * i) + (2 * i) + 3;

        blockchain.chain[i].shardHash = data[index + 1];
        blockchain.chain[i].prevShardHash = data[index + 2];

        for(int j = 0; j < blockchain.length; j++) {
            int blockIndex = (index + 2) + j * 3;   // start at index + 2 -> (index + 2) + (j * 3) + 1, 2, or 3
            blockchain.chain[i].shard[j].blockHash = data[blockIndex + 1];
            blockchain.chain[i].shard[j].prevBlockHash = data[blockIndex + 2];
            blockchain.chain[i].shard[j].time = data[blockIndex + 3];
        }
    }

    // Calculate the size up to here and fill the partial shard
    unsigned int fullSize = (blockchain.chain.size() * blockchain.length * 3) + (blockchain.chain.size() * 2) + 5;
    blockchain.chain[blockchain.chain.size()].shardHash = data[fullSize - 1];
    blockchain.chain[blockchain.chain.size()].prevShardHash = data[fullSize];

    int remainingBlocks = data.size() - fullSize;
    for(int i = 0; i < remainingBlocks; i++) {
        blockchain.chain[blockchain.chain.size()].blockHash = data[remainingBlocks + (i * 3) + 1];
        blockchain.chain[blockchain.chain.size()].prevBlockHash = data[remainingBlocks + (i * 3) + 2];
        blockchain.chain[blockchain.chain.size()].time = data[remainingBlocks + (i * 3) + 3];
    }
}

void SaveBlockchain::read(Blockchain* chain, std::string filepath) {
    std::vector<std::string> data = reader.getData(&filepath);

    chain.id = data[0];                   // The blockchain unique identifier
    chain.length = stoul(data[1]);        // The shard length
    chain.numOfShards = stoul(data[2]);   // The number of shards in the chain

    // numOfShards - 1 to negate the last shard (last shard may not be full)
    for(int i = 1; i <= (chain.numOfShards - 1); i++) {
        unsigned int index = (chain.length * 3 * i) + (2 * i) + 3;

        chain.chain[i].shardHash = data[index + 1];
        chain.chain[i].prevShardHash = data[index + 2];

        for(int j = 0; j < chain.length; j++) {
            int blockIndex = (index + 2) + j * 3;   // start at index + 2 -> (index + 2) + (j * 3) + 1, 2, or 3
            chain.chain[i].shard[j].blockHash = data[blockIndex + 1];
            chain.chain[i].shard[j].prevBlockHash = data[blockIndex + 2];
            chain.chain[i].shard[j].time = data[blockIndex + 3];
        }
    }

    // Calculate the size up to here and fill the partial shard
    unsigned int fullSize = (chain.chain.size() * chain.length * 3) + (chain.chain.size() * 2) + 5;
    chain.chain[chain.chain.size()].shardHash = data[fullSize - 1];
    chain.chain[chain.chain.size()].prevShardHash = data[fullSize];

    int remainingBlocks = data.size() - fullSize;
    for(int i = 0; i < remainingBlocks; i++) {
        chain.chain[chain.chain.size()].blockHash = data[remainingBlocks + (i * 3) + 1];
        chain.chain[chain.chain.size()].prevBlockHash = data[remainingBlocks + (i * 3) + 2];
        chain.chain[chain.chain.size()].time = data[remainingBlocks + (i * 3) + 3];
    }
}

void SaveBlockchain::save() {
    std::vector<std::string> data;

    data.push_back(blockchain.id);
    data.push_back(std::to_string(blockchain.length));
    data.push_back(std::to_string(blockchain.numOfShards));

    for(int i = 1; i <= (blockchain.numOfShards - 1); i++) {
        unsigned int index = (blockchain.length * 3 * i) + (2 * i) + 3;

        data[index + 1] = blockchain.chain[i].shardHash;
        data[index + 2] = blockchain.chain[i].prevShardHash;

        for(int j = 0; j < blockchain.length; j++) {
            int blockIndex = (index + 2) + j * 3;   // start at index + 2 -> (index + 2) + (j * 3) + 1, 2, or 3
            data[blockIndex + 1] = blockchain.chain[i].shard[j].blockHash;
            data[blockIndex + 2] = blockchain.chain[i].shard[j].prevBlockHash;
            data[blockIndex + 3] = blockchain.chain[i].shard[j].time;
        }
    }

    unsigned int fullSize = (blockchain.chain.size() * blockchain.length * 3) + (blockchain.chain.size() * 2) + 5;
    data[fullSize - 1] = blockchain.chain[blockchain.chain.size()].shardHash;
    data[fullSize] = blockchain.chain[blockchain.chain.size()].prevShardHash;

    int remainingBlocks = ((blockchain.chain.size() * 2) + (blockchain.chain.size() * 3 * blockchain.length) + 3) - fullSize;
    for(int i = 0; i < remainingBlocks; i++) {
        data[remainingBlocks + (i * 3) + 1] = blockchain.chain[blockchain.chain.size()].blockHash;
        data[remainingBlocks + (i * 3) + 2] = blockchain.chain[blockchain.chain.size()].prevBlockHash;
        data[remainingBlocks + (i * 3) + 3] = blockchain.chain[blockchain.chain.size()].time;
    }

    writer.createFile(data, &path);
}

void SaveBlockchain::save(Blockchain* chain, std::string filepath) {
    std::vector<std::string> data;

    data.push_back(chain.id);
    data.push_back(std::to_string(chain.length));
    data.push_back(std::to_string(chain.numOfShards));

    for(int i = 1; i <= (chain.numOfShards - 1); i++) {
        unsigned int index = (chain.length * 3 * i) + (2 * i) + 3;

        data[index + 1] = chain.chain[i].shardHash;
        data[index + 2] = chain.chain[i].prevShardHash;

        for(int j = 0; j < chain.length; j++) {
            int blockIndex = (index + 2) + j * 3;   // start at index + 2 -> (index + 2) + (j * 3) + 1, 2, or 3
            data[blockIndex + 1] = chain.chain[i].shard[j].blockHash;
            data[blockIndex + 2] = chain.chain[i].shard[j].prevBlockHash;
            data[blockIndex + 3] = chain.chain[i].shard[j].time;
        }
    }

    unsigned int fullSize = (chain.chain.size() * chain.length * 3) + (chain.chain.size() * 2) + 5;
    data[fullSize - 1] = chain.chain[chain.chain.size()].shardHash;
    data[fullSize] = chain.chain[chain.chain.size()].prevShardHash;

    int remainingBlocks = ((chain.chain.size() * 2) + (chain.chain.size() * 3 * chain.length) + 3) - fullSize;
    for(int i = 0; i < remainingBlocks; i++) {
        data[remainingBlocks + (i * 3) + 1] = chain.chain[chain.chain.size()].blockHash;
        data[remainingBlocks + (i * 3) + 2] = chain.chain[chain.chain.size()].prevBlockHash;
        data[remainingBlocks + (i * 3) + 3] = chain.chain[chain.chain.size()].time;
    }

    writer.createFile(data, &filepath);
}