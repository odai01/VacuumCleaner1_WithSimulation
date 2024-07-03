#include "Vacuum.h"
#include <iostream>

// House class methods

House::House(std::tuple<int, int> Dock_station, std::vector<std::vector<int>>& house_Matrix)
    : Dock_station(Dock_station), house_Matrix(house_Matrix) {}

std::tuple<int, int> House::getDockingStation() const {
    return Dock_station;
}
std::vector<std::vector<int>>& House::getHouseMatrix() {
    return house_Matrix;
}

void House::setDockingStation(const std::tuple<int, int>& Dock_station) {
    this->Dock_station = Dock_station;
}

void House::setMat(const std::vector<std::vector<int>>& house_Matrix) {
    this->house_Matrix = house_Matrix;
}

// Vacuum class methods

Vacuum::Vacuum(int maxBattery,int maxIter,std::tuple<int, int> starting_position, House& house)
    : maxBattery(maxBattery),maxIter(maxIter),current_Position(starting_position), current_Battery(maxBattery), StepsCnt(0),distToDock(0) ,house(house) {}

std::tuple<int, int> Vacuum::getCurrentPosition() const {
    return current_Position;
    
}

int Vacuum::getCurrentBattery() const {
    return current_Battery;
}

int Vacuum::getStepsCnt() const {
    return StepsCnt;
}
int Vacuum::getDistToDock() const{
    return distToDock;
}
void Vacuum::incrementDisToDock(){
    distToDock++;
}
void Vacuum::decreaseDisToDock(){
    distToDock--;
}
std::queue<std::tuple<int, int>> Vacuum::getStepsHistory() const {
    return steps_History;
}
std::queue<char>& Vacuum::getDirectionsHistory() {
    return directionsHistory;
}
int Vacuum::getmaxBattery()const{   
    return maxBattery;
}
House& Vacuum::getHouse(){
    return house;
}
int Vacuum::getmaxIter() const{
    return maxIter;
}
bool Vacuum::isHouseClean(){
    for(const auto& row : house.getHouseMatrix()){
        for(int dirt:row){
            if(dirt>0){return false;}
        }
    }
    return true;
}
bool Vacuum::isWall(char direction){
    int x,y;
    std::tie(x,y) = current_Position;
    switch(direction){
        case 'N':x--;break;
        case 'S':x++;break;
        case 'W':y--;break;
        case 'E':y++;break;
    }

    int rows = static_cast<int>(house.getHouseMatrix().size());
    int cols = static_cast<int>(house.getHouseMatrix()[0].size());
    if(x<0 || y<0 || x>=rows || y>=cols){
        return true;
    } 

    return house.getHouseMatrix()[x][y]==-1;

}

void Vacuum::clean(){
    int x,y;
    std::tie(x,y) = current_Position;

    if(house.getHouseMatrix()[x][y] > 0){
        house.getHouseMatrix()[x][y]--;
        //notice that we dont need to increment StepsCnt, since care to do that  
        //  when calling move('O')
    }
}

bool Vacuum::needCharging(){
    return current_Battery <= (distToDock+1);
}
void Vacuum::charge(){
    // if the max battery is less than 20, we still wanna charge with at least 1 every time.
    int increment=std::max(1,maxBattery / 20);
    current_Battery = std::min(maxBattery, current_Battery + increment);
    steps_History.push(current_Position);
    directionsHistory.push('O');
    StepsCnt++;
}

void Vacuum::Move(char direction) {
    
    int x, y;
    std::tie(x, y) = current_Position;
    
    switch (direction) {
        case 'N':
            x--;
            break;
        case 'S':
            x++;
            break;
        case 'E':
            y++;
            break;
        case 'W':
            y--;
            break;
        case 'O':
            break;
        default:
            return;
    }

    current_Position = std::make_tuple(x, y);
    steps_History.push(current_Position); 
    directionsHistory.push(direction);
    StepsCnt++;
    current_Battery--;
}
