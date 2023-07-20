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

std::pair<bool, TransactionInfo> WalletFunctions::sendTransaction(bool singleTransaction, int numOfTransactions, std::string receiver, double amount, bool onionRouting, int numOfOnionNodes) {
    
}

std::pair<bool, TransactionInfo> WalletFunctions::sendTransaction(bool singleTransaction, int numOfTransactions, std::vector<std::string> receiver, std::vector<double> amount, bool onionRouting, int numOfOnionNodes) {

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
