#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <stdexcept>
#include "Vacuum.h"
#include "CleaningAlgo.h"

struct ParsedData {
    House house;
    int maxBattery;
    int maxIter;
};

class ParseException : public std::runtime_error {
public:
    explicit ParseException(const std::string& message) : std::runtime_error(message) {}
};

ParsedData parseFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<std::vector<int>> matrix;
    std::tuple<int, int> docking_station;
    int maxBattery = 0;
    int maxIter = 0;
    int maxCols = 0;

    if (!file.is_open()) {
        throw ParseException("Failed to open file: " + filename);
    }

    // reading the input and determining the max num of columns
    while (std::getline(file, line)) {
        if (line.empty()) {
            break;
        }
        std::istringstream iss(line);
        std::vector<int> row;
        int value;
        std::string token;
        while (iss >> token) {
            try {
                value = std::stoi(token);
            } catch (std::invalid_argument&) {
                throw ParseException("Non-integer value in house matrix: " + token);
            } catch (std::out_of_range&) {
                throw ParseException("Value out of (int)range in house matrix: " + token);
            }
            if (value < -1 || value > 9) {
                throw ParseException("Invalid value in house matrix: " + std::to_string(value));
            }
            row.push_back(value);
        }
        maxCols = std::max(maxCols, static_cast<int>(row.size()));
        matrix.push_back(row);
    }

    // padding rows to ensure that each row has the same number of columns
    for (auto& row : matrix) {
        while (static_cast<int>(row.size()) < maxCols) {
            row.push_back(-1); // Pad with -1(wall)
        }
    }

    // reading the docking station
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        int x, y;
        if (!(iss >> x >> y)) {
            throw ParseException("Invalid docking station format");
        }
        if (x < 0 || x >= static_cast<int>(matrix.size()) || y < 0 || y >= static_cast<int>(matrix[0].size()) || matrix[x][y] == -1) {
            throw ParseException("Docking station cannot be placed on a wall or outside the matrix");
        }
        docking_station = std::make_tuple(x + 1, y + 1);  // we adjust the coordinates
    } else {
        throw ParseException("Error: Docking station not found in file: " + filename);
    }

    // reading max battery
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        if (!(iss >> maxBattery) || maxBattery <= 0) {
            throw ParseException("Invalid max battery format or value");
        }
    } else {
        throw ParseException("Error: Max battery not found in file: " + filename);
    }

    // reading max iterations
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        if (!(iss >> maxIter) || maxIter < 0) {
            throw ParseException("Invalid max iterations format or value");
        }
    } else {
        throw ParseException("Error: Max iterations not found in file: " + filename);
    }

    file.close();

    // we always create a new matrix with extra rows and columns filled with -1
    // meaning we always pad around input walls
    int rows = matrix.size();
    int cols = maxCols;
    std::vector<std::vector<int>> newMatrix(rows + 2, std::vector<int>(cols + 2, -1));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            newMatrix[i + 1][j + 1] = matrix[i][j];
        }
    }

    House house(docking_station, newMatrix);
    return {house, maxBattery, maxIter};
}

void writeOutput(const std::string& outputFilename, Vacuum& vacuum) {
    std::ofstream outFile(outputFilename);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open output file: " << outputFilename << std::endl;
        return;
    }

    // 1.writing all the steps performed by the vacuum cleaner
    std::queue<std::tuple<int, int>> stepsHistory = vacuum.getStepsHistory();
    while (!stepsHistory.empty()) {
        auto [x, y] = stepsHistory.front();
        outFile << "(" << x << ", " << y << ")\n";
        stepsHistory.pop();
    }

    // 2.writing the total number of steps performed
    outFile << "Total Steps: " << vacuum.getStepsCnt() << "\n";

    // 3.writing the amount of dirt left in the house
    int dirtLeft = 0;
    auto& houseMatrix = vacuum.getHouse().getHouseMatrix();
    for (const auto& row : houseMatrix) {
        for (int dirt : row) {
            if (dirt != -1) {
                dirtLeft += dirt;
            }
        }
    }
    outFile << "Dirt Left: " << dirtLeft << "\n";

    // 4.writing whether the vacuum cleaner is dead
    bool isDead = vacuum.getCurrentBattery() <= 0;
    outFile << "Vacuum Dead: " << (isDead ? "Yes" : "No") << "\n";

    // 5.writing whether the mission is succeeded
    bool missionSucceeded = (dirtLeft == 0) && (vacuum.getCurrentPosition() == vacuum.getHouse().getDockingStation());
    outFile << "Mission Succeeded: " << (missionSucceeded ? "Yes" : "No") << "\n";

    outFile.close();
}

//the next two functions:writeStepsHistory and writeHouseMatrix are for creating 
//json files for the visualize.py
void writeStepsHistory(const std::string& filename, const std::queue<std::tuple<int, int>>& stepsHistory, const House& house) {
    std::ofstream outFile(filename);
    auto dock = house.getDockingStation();
    outFile << "[\n";
    outFile << "  {\"x\": " << std::get<0>(dock) << ", \"y\": " << std::get<1>(dock) << "}";

    auto steps = stepsHistory;
    while (!steps.empty()) {
        outFile << ",\n";
        auto step = steps.front();
        int x, y;
        std::tie(x, y) = step;
        outFile << "  {\"x\": " << x << ", \"y\": " << y << "}";
        steps.pop();
    }
    outFile << "\n]\n";
    outFile.close();
}
void writeHouseMatrix(const std::string& filename, House& house) {
    std::ofstream outFile(filename);
    auto& matrix = house.getHouseMatrix();
    outFile << "[\n";
    for (size_t i = 0; i < matrix.size(); ++i) {
        outFile << "  [";
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            outFile << matrix[i][j];
            if (j < matrix[i].size() - 1) {
                outFile << ", ";
            }
        }
        outFile << "]";
        if (i < matrix.size() - 1) {
            outFile << ",\n";
        }
    }
    outFile << "\n]\n";
    outFile.close();
}


int main(int argc, char* argv[]){
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }
    std::string inputFilename = argv[1];
    try {
        ParsedData data = parseFile(inputFilename);
        House house = data.house;
        int max_Battery = data.maxBattery;
        int max_Iter = data.maxIter;

        writeHouseMatrix("initial_house.json", house);
        Vacuum vacuum(max_Battery, max_Iter, house.getDockingStation(), house);
        CleaningAlgorithm algo;
        algo.startCleaning(vacuum);
        
        std::string outputFilename = "output_" + inputFilename;
        writeOutput(outputFilename, vacuum);
        writeStepsHistory("steps_history.json", vacuum.getStepsHistory(),house);
    } catch (const ParseException& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;//success
}
