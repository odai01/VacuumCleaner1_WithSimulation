# Assignment 1 - Vacuum Cleaner Simulation
## Contributors
- [Odai Agbaria: 212609440, Mohammad mahamid:322776063]

## Overview
This project is a simulation of a vacuum cleaner that navigates a house to clean dirt. The vacuum cleaner's path and behavior are determined by a cleaning algorithm. The simulation reads input from a text file describing the house layout, and it outputs the cleaning steps and results to a text file. Additionally, a Pygame-based visualization is provided to animate the cleaning process.

## Approach
The project consists of several components:
1. **House and Vacuum Classes**: These classes represent the house layout and the vacuum cleaner, respectively(notice that we wrote the house class in the same file with the vacuum class).
2. **Cleaning Algorithm**: the algorithm that guides the vacuum cleaner through the house, cleaning dirt.
3. **Input Parsing**: A function to read and parse the house layout, docking station, maximum battery capacity, and maximum iterations from an input file.
4. **Output Writing**: Functions to write the cleaning steps and results to an output file.
5. **Visualization**: A Pygame-based visualization that animates the cleaning process.

## Input and Output Format
### Input Format
The input file should be structured as follows:
1. **House Matrix**: A matrix representing the house layout. Each row is a line in the file, and each cell is an integer between -1 and 9(the integers must be separated by spaces).
   - `-1` represents a wall.
   - `0` represents an empty(clean) space.(notice that there must be written 0, not space as suggested in the assignment file)
   - Positive integers (1-9) represent the amount of dirt in that cell.
2. **Docking Station**: A line specifying the coordinates of the docking station, given as two integers separated by a space.
3. **Maximum Battery**: A line specifying the maximum battery capacity of the vacuum cleaner.
4. **Maximum Iterations**: A line specifying the maximum number of iterations (steps) the vacuum cleaner can perform.

notice that after the house matrix must be an empty line, and then the docking station.
We highly recommend reading the HLD, in point 3 there we explained also some important things there.

### Output Format
The output file will contain:
1. The steps performed by the vacuum cleaner, each step in the format `(x, y)`,written in a line.
2. The total number of steps performed.
3. The amount of dirt left in the house.
4. Whether the vacuum cleaner is dead (battery depleted).
5. Whether the cleaning mission succeeded (all dirt cleaned and vacuum returned to docking station).

you can see the provided input and output example provided with the assignment files.
## How to Build and Run
1. **Building the Project**: first of all write: make 
    (Ensure you have `g++` version 11.4 or above installed on your system.)
2. **Running the Project**: then write: ./myrobot input_a.txt  
    This will produce an output file named output_input_a.txt and two JSON files.
3. **Running the Visualization**: then write: python visualize.py  
    (Ensure you have Python and Pygame installed on your system)



                              ## Hope you enjoy the simulation ##