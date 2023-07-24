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
};

#endif