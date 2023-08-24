/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-network/blob/main/LICENSE for further license details.        */

#include "WalletFunctions.h"


std::string WalletFunctions::generatePublicKey(std::string privateKey) {
    return ntru.generatePublicKey(privateKey);
}

std::pair<std::string, std::string> WalletFunctions::generateKeyPair(std::string username, std::string password) {
    std::pair<std::string, std::string> output;

    output.first = ntru.generatePrivateKey(username + password);
    output.second = ntru.generatePublicKey(output.first);

    return output;
}

std::pair<std::string, std::string> WalletFunctions::generateStealthKeyPair(std::string publicKey, std::string privateKey, unsigned int index) {
    std::pair<std::string, std::string> output;

    output.first = winternitz.generatePrivateKey(std::to_string(index) + privateKey + privateKey);
    output.second = winternitz.generatePublicKey(output.first);

    return output;
}

std::pair<bool, TransactionInfo> WalletFunctions::sendTransaction(double walletbalance, std::string privateKey, std::string receiver, double amount, unsigned short decoys) {
    // Check that the number of decoys is valid (these min/max still need to be decided)
    if(decoys < 5 || decoys > 20)
        decoys = 10;

    // Check that the wallet has enough balance to cover the transaction amount + potential fee's
    if(getTransactionFee(1) + amount > walletbalance)
        return;

    RingSignature sig = signature.generateRingSignature(amount, receiver, privateKey, decoys);     // Generate the signature

    // Send the ring signature to all the active nodes on the network
    for(int i = 0; i < sync.knownnodes.size(); i++) {   // The number of nodes on the network
        client.connectToServer(knownnodes.ip, knownnodes.port, NULL, std::to_string(sig));
    }

    // Loop until the transaction shows up on chain (gets a cooldown of ___ seconds/minutes)
    int timer = 0;
    while(sync.blockchain.chain.shard.back().ringsignature != sig) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        
        if(timer == 1000)
            return;     // Return Error

        timer++;        // Increment timeout timer
    }

    // Once the transaction is on the blockchain, get the transaction info:
    
}

std::pair<bool, TransactionInfo> WalletFunctions::sendTransaction(double walletbalance, std::string privateKey, std::string receiver, double amount, unsigned short decoys, int numOfOnionNodes) {
    // Check that the number of decoys is valid (these min/max still need to be decided)
    if(decoys < 5 || decoys > 20)
        decoys = 10;

    // Check that the number of onion nodes are valid - if invalid, it will set it to the default of 3
    if(numOfOnionNodes < 3 || numOfOnionNodes > 5)
        numOfOnionNodes = 3;

    // Check that the wallet has enough balance to cover the transaction amount + potential fee's
    if(getTransactionFee(1, numOfOnionNodes) + amount > walletbalance)
        return;

    // Generate the signature
    RingSignature sig = signature.generateRingSignature(amount, receiver, privateKey, decoys);

    // Send the ring signature to all the active nodes on the network using onion routing
    
    // Get the replies from all active nodes, and if the number of transaction successes outweight the number of transaction failed, the transaction has gone through!
    //  -> This takes into account the node bias system

}

std::pair<bool, TransactionInfo> WalletFunctions::sendTransaction(double walletbalance, std::string privateKey, std::vector<std::string> receivers, std::vector<double> amounts, unsigned short decoys) {
    // Check that the number of decoys is valid (these min/max still need to be decided)
    if(decoys < 5 || decoys > 20)
        decoys = 10;

    // Check that the wallet has enough balance to cover the transaction amount + potential fee's
    int totalamount;
    for(int i = 0; i < amounts.size(); i++)
        totalamount = amounts[i] + totalamount;
    if(totalamount + getTransactionFee(amounts.size()) > walletbalance)
        return;

    // Generate the signatures
    std::vector<RingSignature> sigs;
    for(int i = 0; i < receivers.size(); i++)
        sigs.push_back(signature.generateRingSignature(amounts[i], receivers[i], privateKey, decoys));

    // Send the ring signatures to all the active nodes on the network

    // Get the replies from all active nodes, and if the number of transaction successes outweight the number of transaction failed, the transaction has gone through!
    //  -> This takes into account the node bias system

}

std::pair<bool, TransactionInfo> WalletFunctions::sendTransaction(double walletbalance, std::string privateKey, std::vector<std::string> receivers, std::vector<double> amounts, unsigned short decoys, int numOfOnionNodes) {
    // Check that the number of decoys is valid (these min/max still need to be decided)
    if(decoys < 5 || decoys > 20)
        decoys = 10;

    // Check that the number of onion nodes are valid - if invalid, it will set it to the default of 3
    if(numOfOnionNodes < 3 || numOfOnionNodes > 5)
        numOfOnionNodes = 3;

    // Check that the wallet has enough balance to cover the transaction amount + potential fee's
    int totalamount;
    for(int i = 0; i < amounts.size(); i++)
        totalamount = amounts[i] + totalamount;
    if(totalamount + getTransactionFee(amounts.size()) > walletbalance)
        return;

    // Generate the signatures
    std::vector<RingSignature> sigs;
    for(int i = 0; i < receivers.size(); i++)
        sigs.push_back(signature.generateRingSignature(amounts[i], receivers[i], privateKey, decoys));

    // Send the ring signatures to all the active nodes on the network using onion routing

    // Get the replies from all active nodes, and if the number of transaction successes outweight the number of transaction failed, the transaction has gone through!
    //  -> This takes into account the node bias system

}

std::pair<std::string, double> WalletFunctions::getTransactionFee(int numOfTransactions) {
    // getTransactionFee returns a std::pair from the Sync framework - dependant on the number of transactions and number of onion nodes (sync keeps each possible outcome in a vector)
    sync.getTransactionFee(numOfTransactions);
}

std::pair<std::string, double> WalletFunctions::getTransactionFee(int numOfTransactions, int numOfOnionNodes) {
    // Returns a std::pair from the Sync framework - dependant on the number of transactions and number of onion nodes (sync keeps each possible outcome in a vector)
    sync.getTransactionFee(numOfTransactions, numOfOnionNodes);
}

Block WalletFunctions::getBlock(unsigned int shard, unsigned int block) {
    return sync.blockchain.chain[shard].shard[block];
}

std::vector<Block> WalletFunctions::getBlocks(std::vector<unsigned int> shards, std::vector<unsigned int> blocks) {
    std::vector<Block> output;

    if(shards.size() != blocks.size())          // Check that the input vectors are the same size
        return output;                          //    -> Returns the blank output vector

    for(int i = 0; i < shards.size(); i++)      // Fill the output vector with the requested blocks
        output.push_back(sync.blockchain.chain[shards[i]].shard[blocks[i]]);

    return output;
}

std::vector<Block> WalletFunctions::relatedBlocksStealth(std::string stealthKey) {
    std::vector<Block> output;
    for(int i = 0; i < sync.blockchain.chain.size(); i++)                                          // Go through each shard
        for(int j = 0; j < sync.blockchain.chain[i].shard.size(); j++)                             //  -> Each Block
            if(sync.blockchain.chain[i].shard[i].ringsignature.data.receiverkey == stealthKey)     // Check the receiver key
                output.push_back(sync.blockchain.chain[i].shard[i]);

    return output;
}

std::vector<Block> WalletFunctions::relatedBlocksPrivate(std::string privateKey) {
    
}

std::vector<Block> WalletFunctions::relatedBlocksPublic(std::string publicKey) {
    std::vector<Block> output;
    for(int i = 0; i < sync.blockchain.chain.size(); i++)                                                // Go through each shard
        for(int j = 0; j < sync.blockchain.chain[i].shard.size(); j++)                                   // Go through each decoy in the block
            for(int k = 0; k < sync.blockchain.chain[i].shard[i].ringsignature.publicKeys.size(); k++)   // Go through each decoy ring
                if(sync.blockchain.chain[i].shard[i].ringsignature.publicKeys[k] == publicKey)           // Check the public key
                    output.push_back(sync.blockchain.chain[i].shard[i]);

    return output;
}

std::vector<TransactionInfo> WalletFunctions::relatedTransactions(std::string privateKey) {

}

double WalletFunctions::walletBalanceCheck(std::string privateKey) {

}

double WalletFunctions::walletBalanceCheck(std::string privateKey, std::vector<TransactionInfo> transactionRepository) {

}
