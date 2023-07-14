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

std::pair<std::string, double> WalletFunctions::requestTransactionFee(bool singleTransaction, int numOfTransactions, std::string receiver, double amount, bool onionRouting, int numOfOnionNodes) {

}

std::pair<bool, TransactionInfo> WalletFunctions::sendTransaction(bool singleTransaction, int numOfTransactions, std::string receiver, double amount, bool onionRouting, int numOfOnionNodes) {
    
}

std::pair<bool, TransactionInfo> WalletFunctions::sendTransaction(bool singleTransaction, int numOfTransactions, std::vector<std::string> receiver, std::vector<double> amount, bool onionRouting, int numOfOnionNodes) {

}

Block WalletFunctions::getBlock(unsigned int shard, unsigned int block) {

}

std::vector<Block> WalletFunctions::getBlock(unsigned int shard, unsigned int block) {

}

std::vector<Block> WalletFunctions::relatedBlocks(std::string privateKey) {

}

std::vector<TransactionInfo> WalletFunctions::relatedTransactions(std::string privateKey) {

}

double WalletFunctions::walletBalanceCheck(std::string privateKey) {

}

double WalletFunctions::walletBalanceCheck(std::string privateKey, std::vector<TransactionInfo> transactionRepository) {

}
