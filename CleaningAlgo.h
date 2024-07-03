#ifndef CLEANING_ALGORITHM_H
#define CLEANING_ALGORITHM_H

#include "Vacuum.h"

class CleaningAlgorithm {
public:
    CleaningAlgorithm();

    void startCleaning(Vacuum& vacuum);
    bool returnToDocking(Vacuum& vacuum);
};

#endif 
