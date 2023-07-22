#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <vector>


struct Configuration {
    Configuration() {};

    // The Valency Network Configuration Information goes here.
    int minNumOfNodes = 3;
    int minNumOfOnionNodes = 5;
    int maxNumOfOnionNodes = 6;

    // Dynamic - some are set to fixed values - this will be updated in later revisions
    unsigned int networkLoad = 10;    // 1 -> 1000000, with 1 being the least load

    void detectLoad();  // Creates its own thread and constantly updates the networkload variable
};

#endif