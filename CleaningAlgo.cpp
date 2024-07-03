#include "CleaningAlgo.h"
#include <iostream>


CleaningAlgorithm::CleaningAlgorithm() {}

void CleaningAlgorithm::startCleaning(Vacuum& vacuum) {
    const char directions[]={'N','S','W','E','O'}; // O is stay
    std::srand(0);
    while(vacuum.getStepsCnt() < vacuum.getmaxIter() && vacuum.getCurrentBattery() > 0){
    
        if(vacuum.needCharging() || vacuum.isHouseClean()){
            //go to the docking to charge or to finish mission
            returnToDocking(vacuum);
            
            if(vacuum.isHouseClean()){
                if(vacuum.getStepsCnt() < vacuum.getmaxIter()){
                    vacuum.charge();// to make sure the vaccum finishes and he is not dead.
                }
                break;
            }
            while(vacuum.getCurrentBattery() < vacuum.getmaxBattery() && vacuum.getStepsCnt() < vacuum.getmaxIter())  { 
                vacuum.charge();
            }
        }
        else{//no need to charge
            int x,y;
            std::tie(x,y) = vacuum.getCurrentPosition();
            char direction;
            if(vacuum.getHouse().getHouseMatrix()[x][y]>0){
                // if the current place is dirty we wanna clean it
                direction='O';
                vacuum.Move(direction);
                vacuum.clean();
            }
            else{
                if(vacuum.isWall('E') && vacuum.isWall('W') && vacuum.isWall('N') && vacuum.isWall('S')){
                    //in this case we want the vacuum to stop, even if the steps cnt is smaller than the max iteration
                    //sicne this indicates that the vacuum is stuck
                    break;
                }
                direction=directions[rand() % 4];
                if(vacuum.isWall(direction)==false){
                    vacuum.Move(direction);
                    if(direction!='O'){vacuum.incrementDisToDock();}
                }
            }
        }
    }
    
}
bool CleaningAlgorithm::returnToDocking(Vacuum& vacuum) {
    std::queue<char> directionsHistory = vacuum.getDirectionsHistory();
    std::vector<char> directionsVec;

    // Copy directions to a vector
    while (!directionsHistory.empty()) {
        char direction = directionsHistory.front();
        directionsHistory.pop();
        directionsVec.push_back(direction);
    }

    // Move back to docking station
    for (int i = static_cast<int>(directionsVec.size()) - 1; i >= 0 ; --i) {
        if(vacuum.getCurrentBattery()<0){break;}
        if(vacuum.getStepsCnt()>=vacuum.getmaxIter()){break;}
        char direction = directionsVec[i];
        char reverseDirection;

        switch (direction) {
        case 'N':
            reverseDirection = 'S';
            break;
        case 'S':
            reverseDirection = 'N';
            break;
        case 'E':
            reverseDirection = 'W';
            break;
        case 'W':
            reverseDirection = 'E';
            break;
        default:
            continue;
        }

        // Move in the reverse direction if it is not stay
        if (!vacuum.isWall(reverseDirection) && reverseDirection!='O') {
            vacuum.decreaseDisToDock();
            vacuum.Move(reverseDirection);
        }
        // Check if we reached the docking station
        if (vacuum.getCurrentPosition() == vacuum.getHouse().getDockingStation()) {
            return true;
        }
    }
    return vacuum.getCurrentPosition() == vacuum.getHouse().getDockingStation();
}
