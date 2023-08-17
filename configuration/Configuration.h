/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-network/blob/main/LICENSE for further license details.        */

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