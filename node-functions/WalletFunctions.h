/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-network/blob/main/LICENSE for further license details.        */

#ifndef WALLETFUNCTIONS_H
#define WALLETFUNCTIONS_H

#include <string>
#include <vector>
#include "sync/Sync.h"
#include "local-save/nodes/SaveNodes.h"
#include "local-save/blockchain/SaveBlockchain.h"
#include "valency-core/networking/client/Client.h"
#include "valency-core/networking/onion-routing/Onion.h"
#include "valency-core/file-recognition/file-writer/FileWriter.h"
#include "valency-core/file-recognition/file-reader/FileReader.h"
#include "valency-core/cryptography/ntru-encryption/NTRUencrypt.h"
#include "valency-core/cryptography/aes-encryption/AES-Encryption.h"
#include "valency-core/cryptography/traceable-ring-signatures/Ring-Signature.h"


class WalletFunctions {
private:
    Client client;
    WinternitzSignature winternitz;
    
    Onion onion(2);
    NTRUencrypt ntru(3);
    AESEncryption AES(256);
    TraceableRingSignature signature;
    Sync sync(false, "blockchain.vlnc", "knownnodes.vlnc");

public:
    WalletFunctions() {};

    // Generator Functions
    std::string generatePublicKey(std::string privateKey);
    std::pair<std::string, std::string> generateKeyPair(std::string username, std::string password);                                // Generates an NTRUencyrpt key pair from a username + password (used as seeds)
    std::pair<std::string, std::string> generateStealthKeyPair(std::string publicKey, std::string privateKey, unsigned int index);  // Index is the number of stealth key pairs already generated + 1

    // Transaction Functions
    std::pair<bool, TransactionInfo> sendTransaction(double walletbalance, std::string privateKey, std::string receiver, double amount, unsigned short decoys);                                                     // Single Transaction - no onion routing
    std::pair<bool, TransactionInfo> sendTransaction(double walletbalance, std::string privateKey, std::string receiver, double amount, unsigned short decoys, int numOfOnionNodes);                                // Single Transaction - onion routing
    std::pair<bool, TransactionInfo> sendTransaction(double walletbalance, std::string privateKey, std::vector<std::string> receivers, std::vector<double> amounts, unsigned short decoys);                         // Multi Transaction - no onion routing
    std::pair<bool, TransactionInfo> sendTransaction(double walletbalance, std::string privateKey, std::vector<std::string> receivers, std::vector<double> amounts, unsigned short decoys, int numOfOnionNodes);    // Multi Transaction - onion routing

    // Getter Functions
    std::pair<std::string, double> getTransactionFee(int numOfTransactions);                             // Gets the transaction fee from the Sync Framework - Returns the transaction fee and expiration time (min 30 seconds)
    std::pair<std::string, double> getTransactionFee(int numOfTransactions,  int numOfOnionNodes);       // Gets the transaction fee from the Sync Framework - Returns the transaction fee and expiration time (min 30 seconds) - Onion Routing
    Block getBlock(unsigned int shard, unsigned int block);                                              // Get a specific block from the Sync Framework
    std::vector<Block> getBlocks(std::vector<unsigned int> shards, std::vector<unsigned int> blocks);    // Gets the requested blocks - ensure shard and block vector index match

    // Scanner Functions
    std::vector<Block> relatedBlocksStealth(std::string stealthKey);                                                 // Scans the blockchain and returns a vector of any blocks that are related to the one-time-stealth key inputted (sender, receiver, or used as decoy in TRS!)
    std::vector<Block> relatedBlocksPrivate(std::string privateKey);                                                 // Scans the blockchain and returns a vector of any blocks that are related to the privateKey inputted (sender, receiver, or used as decoy in TRS!)
    std::vector<Block> relatedBlocksPublic(std::string publicKey);                                                 // Scans the blockchain and returns a vector of any blocks that are related to the privateKey inputted (sender, receiver, or used as decoy in TRS!)
    std::vector<TransactionInfo> relatedTransactions(std::string privateKey);                                 // Scans the blockchain and returns a vector of any transactions that are related to the privateKey inputted
    double walletBalanceCheck(std::string privateKey);                                                        // Checks the wallet balance from the blockchain
    double walletBalanceCheck(std::string privateKey, std::vector<TransactionInfo> transactionRepository);    // Checks the wallet balance from the inputted transaction repository
};

#endif