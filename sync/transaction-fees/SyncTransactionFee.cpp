#include "SyncTransactionFee.h"


void SyncTransactionFee::syncExpiry() {
    // Get the current expiry time (last 30s increment)
    auto expirytimeDuration = std::chrono::system_clock::now().time_since_epoch() - (std::chrono::system_clock::now().time_since_epoch() % std::chrono::seconds(30));
    
    std::chrono::system_clock::time_point expirytime{expirytimeDuration};   // Convert to a time point
    auto expirytime_t = std::chrono::system_clock::to_time_t(expirytime);   // Convert to time_t (allows stringstream - translate time into a string)

    while(1) {
        while(std::chrono::system_clock::now() - std::chrono::seconds(29) > expirytime) {   // Goes into loop @ 29s and then constantly loops to check for 30s (to reduce cpu overhead)
            while(std::chrono::system_clock::now() - std::chrono::seconds(30) >= expirytime) {
                expirytime = expirytime + std::chrono::seconds(30);                     // Add 30s to the chrono expirytime

                expirytime_t = std::chrono::system_clock::to_time_t(expirytime);        // Convert to time_t
                expiry.clear(); 
                expiry << std::put_time(std::localtime(&expirytime_t), "%Y-%m-%d %X");  // Convert time_t to string stream
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));   // Sleep for 1s
    }
}

SyncTransactionFee::SyncTransactionFee(std::vector<Position4D>* knownNodes) {
    std::thread expirysyncthread(syncExpiry());       // Create a thread that syncs the fee expiry time
    std::thread feesyncthread(sync(knownNodes));      // Create a thread that syncs the fee amounts
}

void SyncTransactionFee::sync(std::vector<Position4D>* knownNodes) {
    
}

std::pair<std::string, double> SyncTransactionFee::getTransactionFee(int numOfTransactions) {
    if(numOfTransactions > 10 || numOfTransactions < 1)     // Invalid Number of Transactions
        return;

    std::pair<std::string, double> output;
    output.first = expiry.str();
    output.second = transactionfees[numOfTransactions];     // The first 10 values in transactionfees vector are non-onion routing
    return output;
}

std::pair<std::string, double> SyncTransactionFee::getTransactionFee(int numOfTransactions, int numOfOnionNodes) {
    if(numOfTransactions > 10 || numOfTransactions < 1 || numOfOnionNodes > 6 || numOfOnionNodes < 3)     // Invalid Number of Transactions
        return;

    std::pair<std::string, double> output;
    output.first = expiry.str();
    output.second = transactionfees[numOfTransactions + (numOfOnionNodes * 10) - 20];
    return output;
}