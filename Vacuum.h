#ifndef VACUUM_H
#define VACUUM_H

#include <tuple>
#include <queue>
#include <vector>

// House class 
class House {
private:
    std::tuple<int, int> Dock_station;
    std::vector<std::vector<int>> house_Matrix;

public:
    House(std::tuple<int, int> Dock_station, std::vector<std::vector<int>>& house_Matrix);

    std::tuple<int, int> getDockingStation() const;
    std::vector<std::vector<int>>& getHouseMatrix();

    void setDockingStation(const std::tuple<int, int>& Dock_station);
    void setMat(const std::vector<std::vector<int>>& house_Matrix);
};


// Vacuum class 
class Vacuum {
private:
    int maxBattery;
    int maxIter;
    std::tuple<int, int> current_Position;
    int current_Battery;
    int StepsCnt;
    int distToDock;
    std::queue<std::tuple<int, int>> steps_History;
    std::queue<char> directionsHistory;
    House& house;

public:
    Vacuum(int maxBattery,int maxIter,std::tuple<int, int> starting_position, House& house);

    std::tuple<int, int> getCurrentPosition() const;
    int getCurrentBattery() const;
    int getStepsCnt() const;
    std::queue<std::tuple<int, int>> getStepsHistory() const;
    std::queue<char>& getDirectionsHistory();
    int getmaxBattery()const;
    int getmaxIter()const;
    int getDistToDock() const;
    House& getHouse();
    void Move(char direction);

    // to check whether a step to the provided direction will lead to a wall
    bool isWall(char direction);

    bool isHouseClean();
    void clean();
    bool needCharging();
    void charge();
    void incrementDisToDock();
    void decreaseDisToDock();

};

#endif 
