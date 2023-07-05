#ifndef WALLETFUNCTIONS_H
#define WALLETFUNCTIONS_H

#include <string>
#include <vector>
#include "structures/Blockchain.h"
#include "valency-core/networking/onion-routing/Onion.h"
#include "valency-core/file-recognition/file-writer/FileWriter.h"
#include "valency-core/file-recognition/file-reader/FileReader.h"
#include "valency-core/cryptography/ntru-encryption/NTRUencrypt.h"
#include "valency-core/cryptography/aes-encryption/AES-Encryption.h"
#include "valency-core/cryptography/traceable-ring-signatures/Ring-Signature.h"


class WalletFunctions {
private:
    std::string blockchainFilePath = "blockchain.vlnc";     // The blockchain file path
    Blockchain blockchain;                                  // The Valency Network Blockchain!

    Onion onion(2);
    FileReader reader;
    FileWriter writer;
    NTRUencrypt ntru(3);
    AESEncryption AES(256);

public:
    WalletFunctions() {};

    // Generator Functions
    std::string generatePublicKey(std::string privateKey);                                                                          // Generates the NTRUencrypt public key from the corresponding private key
    std::pair<std::string, std::string> generateKeyPair(std::string username, std::string password);                                // Generates an NTRUencyrpt key pair from a username + password (used as seeds)
    std::pair<std::string, std::string> generateStealthKeyPair(std::string publicKey, std::string privateKey, unsigned int index);  // Index is the number of stealth key pairs already generated + 1

    // Transaction Functions
    std::pair<std::string, double> requestTransactionFee(bool singleTransaction, int numOfTransactions, std::string receiver, double amount, bool onionRouting, int numOfOnionNodes);   // Returns the transaction fee and expiration time (min 30 seconds)
    std::pair<bool, TransactionInfo> sendTransaction(bool singleTransaction, int numOfTransactions, std::string receiver, double amount, bool onionRouting, int numOfOnionNodes);       // Sends the transaction
    
    // Getter Functions
    Block getBlock(unsigned int shard, unsigned int block);                // Get a specific block from the blockchain
    std::vector<Block> getBlock(unsigned int shard, unsigned int block);   // Used to download the blockchain/parts of the blockchain

    // Scanner Functions
    std::vector<Block> relatedBlocks(std::string privateKey);                                                 // Scans the blockchain and returns a vector of any blocks that are related to the privateKey inputted (sender, receiver, or used as decoy in TRS!)
    std::vector<TransactionInfo> relatedTransactions(std::string privateKey);                                 // Scans the blockchain and returns a vector of any transactions that are related to the privateKey inputted
    double walletBalanceCheck(std::string privateKey);                                                        // Checks the wallet balance from the blockchain
    double walletBalanceCheck(std::string privateKey, std::vector<TransactionInfo> transactionRepository);    // Checks the wallet balance from the inputted transaction repository
};

#endif