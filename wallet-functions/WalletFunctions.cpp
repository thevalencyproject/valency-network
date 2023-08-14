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
    if(decoys < 5 || decoys > 20)     // If there is an invalid amount of decoys, return nothing
        return;

    // Check that the wallet has enough balance to cover the transaction amount + potential fee's
    if(getTransactionFee(1) + amount > walletbalance)
        return;

    RingSignature sig = signature.generateRingSignature(amount, receiver, privateKey, decoys);     // Generate the signature

    // Send the ring signature to all the active nodes on the network
    

    // Get the replies from all active nodes, and if the number of transaction successes outweight the number of transaction failed, the transaction has gone through!
    //  -> This takes into account the node bias system

}

std::pair<bool, TransactionInfo> WalletFunctions::sendTransaction(double walletbalance, std::string privateKey, std::string receiver, double amount, unsigned short decoys, int numOfOnionNodes) {
    // Check that the number of decoys is valid (these min/max still need to be decided)

    // Check that the wallet has enough balance to cover the transaction amount + potential fee's

    RingSignature sig = signature.generateRingSignature(amount, receiver, privateKey, decoys);     // Generate the signature

    // Send the ring signature to all the active nodes on the network using onion routing
    
    // Get the replies from all active nodes, and if the number of transaction successes outweight the number of transaction failed, the transaction has gone through!
    //  -> This takes into account the node bias system

}

std::pair<bool, TransactionInfo> WalletFunctions::sendTransaction(double walletbalance, std::string privateKey, std::vector<std::string> receivers, std::vector<double> amounts, unsigned short decoys) {
    // Check that the number of decoys is valid (these min/max still need to be decided)

    // Check that the wallet has enough balance to cover the transaction amount + potential fee's

    std::vector<RingSignature> sigs;
    for(int i = 0; i < receivers.size(); i++)
        sigs.push_back(signature.generateRingSignature(amounts[i], receivers[i], privateKey, decoys));     // Generate the signatures

    // Send the ring signatures to all the active nodes on the network

    // Get the replies from all active nodes, and if the number of transaction successes outweight the number of transaction failed, the transaction has gone through!
    //  -> This takes into account the node bias system

}

std::pair<bool, TransactionInfo> WalletFunctions::sendTransaction(double walletbalance, std::string privateKey, std::vector<std::string> receivers, std::vector<double> amounts, unsigned short decoys, int numOfOnionNodes) {
    // Check that the number of decoys is valid (these min/max still need to be decided)

    // Check that the wallet has enough balance to cover the transaction amount + potential fee's

    std::vector<RingSignature> sigs;
    for(int i = 0; i < receivers.size(); i++)
        sigs.push_back(signature.generateRingSignature(amounts[i], receivers[i], privateKey, decoys));     // Generate the signatures

    // Send the ring signatures to all the active nodes on the network using onion routing

    // Get the replies from all active nodes, and if the number of transaction successes outweight the number of transaction failed, the transaction has gone through!
    //  -> This takes into account the node bias system

}

std::pair<std::string, double> WalletFunctions::getTransactionFee(int numOfTransactions) {
    // getTransactionFee returns a std::pair from the Sync framework - dependant on the number of transactions and number of onion nodes (sync keeps each possible outcome in a vector)
    sync.getTransactionFee(numOfTransactions);
}

std::pair<std::string, double> WalletFunctions::getTransactionFee(int numOfTransactions, bool onionRouting, int numOfOnionNodes) {
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

std::vector<Block> WalletFunctions::relatedBlocks(std::string privateKey) {

}

std::vector<TransactionInfo> WalletFunctions::relatedTransactions(std::string privateKey) {

}

double WalletFunctions::walletBalanceCheck(std::string privateKey) {

}

double WalletFunctions::walletBalanceCheck(std::string privateKey, std::vector<TransactionInfo> transactionRepository) {

}
