#ifndef WALLETFUNCTIONS_H
#define WALLETFUNCTIONS_H

#include <string>
#include <vector>
#include "local-save/nodes/SaveNodes.h"
#include "local-save/blockchain/SaveBlockchain.h"
#include "valency-core/networking/onion-routing/Onion.h"
#include "valency-core/file-recognition/file-writer/FileWriter.h"
#include "valency-core/file-recognition/file-reader/FileReader.h"
#include "valency-core/cryptography/ntru-encryption/NTRUencrypt.h"
#include "valency-core/cryptography/aes-encryption/AES-Encryption.h"
#include "valency-core/cryptography/traceable-ring-signatures/Ring-Signature.h"


class WalletFunctions {
private:
    Onion onion(2);
    FileReader reader;
    FileWriter writer;
    NTRUencrypt ntru(3);
    AESEncryption AES(256);
    WinternitzSignature winternitz;

    // Readable/Writeable Files
    SaveBlockchain blockchain("blockchain.vlnc");   // The Blockchain
    SaveNodes knownNodes("known-nodes.vlnc");       // The Known Nodes
    SaveNodes activeNodes("active-nodes.vlnc");     // The Nodes that are currently active

public:
    WalletFunctions() {};

    // Constant Refresh Functions (functions run constantly in their own threads)
    void syncBlockchain();      // Syncs the Blockchain up with all nodes on the network
    void syncActiveNodes();     // Syncs any active nodes (pushes address + IP + port into activeNodes vector) - also adds them to knownNodes vector + knownNodes file if they are unknown
    // Server Communicate Functions (for the constant refresh function pointer inputs)
    std::string syncBlockchainServer(std::string input);
    std::string syncActiveNodesServer(std::string input);
    // Client Communicate Functions (for the constant refresh function pointer inputs)
    std::string syncBlockchainClient(std::string input);
    std::string syncActiveNodesClient(std::string input);

    // Generator Functions
    std::string generatePublicKey(std::string privateKey);                                                                          // Generates the NTRUencrypt public key from the corresponding private key
    std::pair<std::string, std::string> generateKeyPair(std::string username, std::string password);                                // Generates an NTRUencyrpt key pair from a username + password (used as seeds)
    std::pair<std::string, std::string> generateStealthKeyPair(std::string publicKey, std::string privateKey, unsigned int index);  // Index is the number of stealth key pairs already generated + 1

    // Transaction Functions
    std::pair<std::string, double> requestTransactionFee(bool singleTransaction, int numOfTransactions, std::string receiver, double amount, bool onionRouting, int numOfOnionNodes);                           // Returns the transaction fee and expiration time (min 30 seconds)
    std::pair<bool, TransactionInfo> sendTransaction(bool singleTransaction, int numOfTransactions, std::string receiver, double amount, bool onionRouting, int numOfOnionNodes);                               // Sends the transaction
    std::pair<bool, TransactionInfo> sendTransaction(bool singleTransaction, int numOfTransactions, std::vector<std::string> receiver, std::vector<double> amount, bool onionRouting, int numOfOnionNodes);     // Sends the multi-transaction (also handles single index vectors for single transactions)

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