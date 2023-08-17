/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-core/blob/main/LICENSE for further license details.        */

#include "SaveBlockchain.h"


SaveBlockchain::SyncBlockchain(std::string filepath) {
    read(filepath);
}

void SaveBlockchain::read(std::string filepath) {
    path = filepath;
    std::vector<std::string> data = reader.getData(&path);

    int blocksRemainingInShard = data[0];      // The number of blocks in the unfilled shard
    blockchain.id = data[1];                   // The blockchain unique identifier
    blockchain.length = stoul(data[2]);        // The shard length
    blockchain.numOfShards = stoul(data[3]);   // The number of shards in the chain

    int index = 4;     // The index counter

    // Filled Shards
    for(int i = 1; i <= (blockchain.numOfShards - 1); i++) {
        blockchain.chain[i].shardHash = data[index];
        blockchain.chain[i].prevShardHash = data[++index];

        // Block
        for(int j = 0; j < blockchain.length; j++) {
            blockchain.chain[i].shard[j].ringsignature.keyImage = data[++index];
            blockchain.chain[i].shard[j].ringsignature.signature = data[++index];

            int blockIndex = ++index;
            for(int k = 0; k < data[blockIndex]; k++)   // Ring of public keys here
                blockchain.chain[i].shard[j].ringsignature.publicKeys.push_back(data[++index]);

            blockchain.chain[i].shard[j].ringsignature.data.receiverKey = data[++index];
            blockchain.chain[i].shard[j].ringsignature.data.transactionAmount = stoul(data[++index]);

            blockchain.chain[i].shard[j].blockHash = data[++index];
            blockchain.chain[i].shard[j].prevBlockHash = data[++index];
            blockchain.chain[i].shard[j].time = data[++index];
        }
    }

    // The unfilled shard (on the end of the blockchain)
    blockchain.chain[blockchain.chain.size()].shardHash = data[++index];
    blockchain.chain[blockchain.chain.size()].prevShardHash = data[++index];

    // Loop through the remaining blocks in the unfilled shard
    for(int i = 0; i < blocksRemainingInShard; i++) {
        blockchain.chain[blockchain.chain.size()].shard[i].ringsignature.keyImage = data[++index];
        blockchain.chain[blockchain.chain.size()].shard[i].ringsignature.signature = data[++index];

        int blockIndex = ++index;
        for(int j = 0; j < data[blockIndex]; j++)
            blockchain.chain[blockchain.chain.size()].shard[i].ringsignature.publicKeys.push_back(data[++index]);

        blockchain.chain[blockchain.chain.size()].shard[i].ringsignature.data.receiverKey = data[++index];
        blockchain.chain[blockchain.chain.size()].shard[i].ringsignature.data.transactionAmount = data[++index];

        blockchain.chain[blockchain.chain.size()].shard[i].blockHash = data[++index];
        blockchain.chain[blockchain.chain.size()].shard[i].prevBlockHash = data[++index];
        blockchain.chain[blockchain.chain.size()].shard[i].time = data[++index];
    }
}

void SaveBlockchain::read(Blockchain* chain, std::string filepath) {
    std::vector<std::string> data = reader.getData(&filepath);

    int blocksRemainingInShard = data[0];      // The number of blocks in the unfilled shard
    chain.id = data[1];                   // The blockchain unique identifier
    chain.length = stoul(data[2]);        // The shard length
    chain.numOfShards = stoul(data[3]);   // The number of shards in the chain

    int index = 4;     // The index counter

    // Filled Shards
    for(int i = 1; i <= (chain.numOfShards - 1); i++) {
        chain.chain[i].shardHash = data[index];
        chain.chain[i].prevShardHash = data[++index];

        // Block
        for(int j = 0; j < chain.length; j++) {
            chain.chain[i].shard[j].ringsignature.keyImage = data[++index];
            chain.chain[i].shard[j].ringsignature.signature = data[++index];

            int blockIndex = ++index;
            for(int k = 0; k < data[blockIndex]; k++)   // Ring of public keys here
                chain.chain[i].shard[j].ringsignature.publicKeys.push_back(data[++index]);

            chain.chain[i].shard[j].ringsignature.data.receiverKey = data[++index];
            chain.chain[i].shard[j].ringsignature.data.transactionAmount = stoul(data[++index]);

            chain.chain[i].shard[j].blockHash = data[++index];
            chain.chain[i].shard[j].prevBlockHash = data[++index];
            chain.chain[i].shard[j].time = data[++index];
        }
    }

    // The unfilled shard (on the end of the blockchain)
    chain.chain[chain.chain.size()].shardHash = data[++index];
    chain.chain[chain.chain.size()].prevShardHash = data[++index];

    // Loop through the remaining blocks in the unfilled shard
    for(int i = 0; i < blocksRemainingInShard; i++) {
        chain.chain[chain.chain.size()].shard[i].ringsignature.keyImage = data[++index];
        chain.chain[chain.chain.size()].shard[i].ringsignature.signature = data[++index];

        int blockIndex = ++index;
        for(int j = 0; j < data[blockIndex]; j++)
            chain.chain[chain.chain.size()].shard[i].ringsignature.publicKeys.push_back(data[++index]);

        chain.chain[chain.chain.size()].shard[i].ringsignature.data.receiverKey = data[++index];
        chain.chain[chain.chain.size()].shard[i].ringsignature.data.transactionAmount = data[++index];

        chain.chain[chain.chain.size()].shard[i].blockHash = data[++index];
        chain.chain[chain.chain.size()].shard[i].prevBlockHash = data[++index];
        chain.chain[chain.chain.size()].shard[i].time = data[++index];
    }
}

void SaveBlockchain::save() {
    std::vector<std::string> data;

    int blocksRemainingInShard = blockchain.chain.back().shard.size();
    data.push_back(blockchain.id);
    data.push_back(std::to_string(blockchain.length));
    data.push_back(std::to_string(blockchain.numOfShards));

    // Filled Shards
    for(int i = 1; i <= (blockchain.numOfShards - 1); i++) {
        data.push_back(blockchain.chain[i].shardHash);
        data.push_back(blockchain.chain[i].prevShardHash);

        // Block
        for(int j = 0; j < blockchain.length; j++) {
            data.push_back(blockchain.chain[i].shard[j].ringsignature.keyImage);
            data.push_back(blockchain.chain[i].shard[j].ringsignature.signature);

            data.push_back(blockchain.chain[i].shard[j].ringsignature.publicKeys.size());
            for(int k = 0; k < blockchain.chain[i].shard[j].ringsignature.publicKeys.size(); k++)   // Ring of public keys here
                data.push_back(blockchain.chain[i].shard[j].ringsignature.publicKeys[k]);

            data.push_back(blockchain.chain[i].shard[j].ringsignature.data.receiverKey);
            data.push_back(std::to_string(blockchain.chain[i].shard[j].ringsignature.data.transactionAmount));

            data.push_back(blockchain.chain[i].shards[j].blockHash);
            data.push_back(blockchain.chain[i].shards[j].prevBlockHash);
            data.push_back(blockchain.chain[i].shards[j].time);
        }
    }

    // The unfilled shard (on the end of the blockchain)
    data.push_back(blockchain.chain[blockchain.chain.size()].shardHash);
    data.push_back(blockchain.chain[blockchain.chain.size()].prevShardHash);

    // Loop through the remaining blocks in the unfilled shard
    for(int i = 0; i < blocksRemainingInShard; i++) {
        data.push_back(blockchain.chain[blockchain.chain.size()].shard[i].ringsignature.keyImage);
        data.push_back(blockchain.chain[blockchain.chain.size()].shard[i].ringsignature.signature);

        for(int j = 0; j < blockchain.chain[blockchain.chain.size()].shard[i].ringsignature.publicKeys.size(); j++)
            data.push_back(blockchain.chain[blockchain.chain.size()].shard[i].ringsignature.publicKeys[j]);

        data.push_back(blockchain.chain[blockchain.chain.size()].shard[i].ringsignature.data.receiverKey);
        data.push_back(std::to_string(blockchain.chain[blockchain.chain.size()].shard[i].ringsignature.data.transactionAmount));

        data.push_back(blockchain.chain[blockchain.chain.size()].shard[i].blockHash);
        data.push_back(blockchain.chain[blockchain.chain.size()].shard[i].prevBlockHash);
        data.push_back(blockchain.chain[blockchain.chain.size()].shard[i].time);
    }

    writer.createFile(data, &path);
}

void SaveBlockchain::save(Blockchain* chain, std::string filepath) {
    std::vector<std::string> data;

    int blocksRemainingInShard = chain.chain.back().shard.size();
    data.push_back(chain.id);
    data.push_back(std::to_string(chain.length));
    data.push_back(std::to_string(chain.numOfShards));

    // Filled Shards
    for(int i = 1; i <= (chain.numOfShards - 1); i++) {
        data.push_back(chain.chain[i].shardHash);
        data.push_back(chain.chain[i].prevShardHash);

        // Block
        for(int j = 0; j < chain.length; j++) {
            data.push_back(chain.chain[i].shard[j].ringsignature.keyImage);
            data.push_back(chain.chain[i].shard[j].ringsignature.signature);

            data.push_back(chain.chain[i].shard[j].ringsignature.publicKeys.size());
            for(int k = 0; k < chain.chain[i].shard[j].ringsignature.publicKeys.size(); k++)   // Ring of public keys here
                data.push_back(chain.chain[i].shard[j].ringsignature.publicKeys[k]);

            data.push_back(chain.chain[i].shard[j].ringsignature.data.receiverKey);
            data.push_back(std::to_string(chain.chain[i].shard[j].ringsignature.data.transactionAmount));

            data.push_back(chain.chain[i].shards[j].blockHash);
            data.push_back(chain.chain[i].shards[j].prevBlockHash);
            data.push_back(chain.chain[i].shards[j].time);
        }
    }

    // The unfilled shard (on the end of the blockchain)
    data.push_back(chain.chain[chain.chain.size()].shardHash);
    data.push_back(chain.chain[chain.chain.size()].prevShardHash);

    // Loop through the remaining blocks in the unfilled shard
    for(int i = 0; i < blocksRemainingInShard; i++) {
        data.push_back(chain.chain[chain.chain.size()].shard[i].ringsignature.keyImage);
        data.push_back(chain.chain[chain.chain.size()].shard[i].ringsignature.signature);

        for(int j = 0; j < chain.chain[chain.chain.size()].shard[i].ringsignature.publicKeys.size(); j++)
            data.push_back(chain.chain[chain.chain.size()].shard[i].ringsignature.publicKeys[j]);

        data.push_back(chain.chain[chain.chain.size()].shard[i].ringsignature.data.receiverKey);
        data.push_back(std::to_string(chain.chain[chain.chain.size()].shard[i].ringsignature.data.transactionAmount));

        data.push_back(chain.chain[chain.chain.size()].shard[i].blockHash);
        data.push_back(chain.chain[chain.chain.size()].shard[i].prevBlockHash);
        data.push_back(chain.chain[chain.chain.size()].shard[i].time);
    }

    writer.createFile(data, &filepath);
}