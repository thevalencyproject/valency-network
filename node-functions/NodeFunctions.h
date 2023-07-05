#ifndef NODEFUNCTIONS_H
#define NODEFUNCTIONS_H

#include <string>
#include <vector>
#include "structures/Blockchain.h"
#include "valency-core/custom-types/Position.h"
#include "valency-core/networking/client/Client.h"
#include "valency-core/networking/server/Server.h"
#include "valency-core/networking/onion-routing/Onion.h"
#include "valency-core/algorithms/quicksort/Quicksort.h"
#include "valency-core/file-recognition/file-writer/FileWriter.h"
#include "valency-core/file-recognition/file-reader/FileReader.h"
#include "valency-core/cryptography/ntru-encryption/NTRUencrypt.h"
#include "valency-core/cryptography/aes-encryption/AES-Encryption.h"
#include "valency-core/algorithm/lzma-compression/LZMA-Compression.h"
#include "valency-core/cryptography/traceable-ring-signatures/Ring-Signature.h"


class NodeFunctions {
private:
    std::string blockchainFilePath = "blockchain.vlnc";     // The blockchain file path
    Blockchain blockchain;

    Server server;
    Client client;
    Quicksort sort;
    OnionNode onion;
    FileReader reader;
    FileWriter writer;
    NTRUencrypt ntru(3);
    AESEncryption AES(256);

    // Block Buffers
    std::vector<Block> incomingBlocks;     // The incoming block buffer: Blocks that are yet to be verified locally (can be verified by network before being verified locally)
    std::vector<Block> outgoingBlocks;     // The Outgoing Block buffer: Blocks verified locally but not verified network wide (network has not yet come to consensus)
    std::vector<Block> invalidBlocks;      // The Malicious Blocks buffer: Blocks that return invalid during validation come here

    // Other Nodes
    std::vector<Position3D> knownNodes;
    std::vector<Position3D> activeNodes;

    // Malware Detection
    std::vector<Position3D> maliciousSignatures;

public:
    NodeFunctions() {};

    // Constant Refresh Functions (functions run constantly in their own threads)
    void syncBlockchain();          // Syncs the Blockchain up with all other nodes on the network
    void syncActiveNodes();         // Syncs any active nodes (pushes address + IP + port into activeNodes vector) - also adds them to knownNodes vector + knownNodes file if they are unknown
    void syncUnVerifiedBlocks();    // Syncs any unverified blocks from other nodes
    void syncVerifiedBlocks();      // Syncs any valid blocks from the outgoingNodes vector (also gets valid blocks from other nodes)
    void processIncomingBlocks();   // Verifies any blocks in the incomingBlocks vector - loop every ~50ms (estimate): verified blocks go to outgoingBlocks vector - invalid verification go to invalidBlocks vector
    void processInvalidBlocks();    // See's if any invalid blocks are malicious and takes action to ban similar blocks in the future
    void syncInvalidBlocks();       // Syncs any invalid blocks from other nodes to filter out similar malicious blocks network wide

    // File Functions
    void saveBlockchain();  // Saves the blockchain file (FileWriter)
    void readBlockchain();  // Reads the blockchain file (FileReader)
    void saveKnownNodes();  // Saves the known nodes file (FileWriter)
    void readKnownNodes();  // Reads the known nodes file (FileReader)

    // Verification Functions
    bool verifyBlock();                     // Verifies the block - returns true if the block is valid
    bool verifyTraceableRingSignature();    // Verifies the ring signature stored inside of the block - returns true if the signature is valid
};

#endif