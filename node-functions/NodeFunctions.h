#ifndef NODEFUNCTIONS_H
#define NODEFUNCTIONS_H

#include <string>
#include <vector>
#include <thread>
#include "sync/Sync.h"
#include "local-save/nodes/SaveNodes.h"
#include "structures/NodeTransactionInfo.h"
#include "valency-core/custom-types/Position.h"
#include "local-save/blockchain/SaveBlockchain.h"
#include "valency-core/networking/client/Client.h"
#include "valency-core/networking/server/Server.h"
#include "valency-core/networking/onion-routing/Onion.h"
#include "valency-core/algorithms/quicksort/Quicksort.h"
#include "valency-core/file-recognition/file-writer/FileWriter.h"
#include "valency-core/file-recognition/file-reader/FileReader.h"
#include "valency-core/cryptography/ntru-encryption/NTRUencrypt.h"
#include "valency-core/cryptography/aes-encryption/AES-Encryption.h"
#include "valency-core/algorithms/lzma-compression/LZMA-Compression.h"
#include "valency-core/cryptography/traceable-ring-signatures/Ring-Signature.h"


class NodeFunctions {
private:
    Server server;
    Client client;
    Quicksort sort;
    OnionNode onion;
    FileReader reader;
    FileWriter writer;
    LZMACompression lzma;
    
    NTRUencrypt ntru(3);
    AESEncryption AES(256);
    Sync sync(true, "blockchain.vlnc", "activenodes.vlnc", "knownnodes.vlnc");

    // Block Buffers
    std::vector<Block> incomingBlocks;     // The incoming block buffer: Blocks that are yet to be verified locally (can be verified by network before being verified locally)
    std::vector<Block> outgoingBlocks;     // The Outgoing Block buffer: Blocks verified locally but not verified network wide (network has not yet come to consensus)
    std::vector<Block> invalidBlocks;      // The Malicious Blocks buffer: Blocks that return invalid during validation come here

    // Constant Refresh Functions (functions run constantly in their own threads)
    void syncUnVerifiedBlocks();    // Syncs any unverified blocks from other nodes
    void syncVerifiedBlocks();      // Syncs any valid blocks from the outgoingNodes vector (also gets valid blocks from other nodes)
    void processIncomingBlocks();   // Verifies any blocks in the incomingBlocks vector - loop every ~50ms (estimate): verified blocks go to outgoingBlocks vector - invalid verification go to invalidBlocks vector
    void processInvalidBlocks();    // See's if any invalid blocks are malicious and takes action to ban similar blocks in the future
    void syncInvalidBlocks();       // Syncs any invalid blocks from other nodes to filter out similar malicious blocks network wide
    void syncKnownTransactions();   // Syncs any known transactions details (for transactions verified by the node) - acts as references for the node
    void processSmartContracts();   // Processes any smart contracts stored on the network
    // Server Communicate Functions (for the constant refresh function pointer inputs)
    std::string syncUnVerifiedBlocksServer(std::string input);
    std::string syncVerifiedBlocksServer(std::string input);
    std::string processIncomingBlocksServer(std::string input);
    std::string processInvalidBlocksServer(std::string input);
    std::string syncInvalidBlocksServer(std::string input);
    std::string syncKnownTransactionsServer(std::string input);
    std::string processSmartContractsServer(std::string input);
    // Client Communicate Functions (for the constant refresh function pointer inputs)
    std::string syncUnVerifiedBlocksClient(std::string input);
    std::string syncVerifiedBlocksClient(std::string input);
    std::string processIncomingBlocksClient(std::string input);
    std::string processInvalidBlocksClient(std::string input);
    std::string syncInvalidBlocksClient(std::string input);
    std::string syncKnownTransactionsClient(std::string input);
    std::string processSmartContractsClient(std::string input);

    // Verification Functions
    bool verifyBlock();                     // Verifies the block - returns true if the block is valid
    bool verifyTraceableRingSignature();    // Verifies the ring signature stored inside of the block - returns true if the signature is valid

    // UI Information Functions
    NodeTransactionDetails latestTransactionDetails;   // Stores the transaction details from the block that the node has validated

public:
    NodeFunctions() {};
    void run();     // Runs the node

    // Getter Functions
    NodeTransactionInfo getNodeTransactionInfo();   // Gets the KnownTransactionDetails for the last transaction the Node processed
};

#endif