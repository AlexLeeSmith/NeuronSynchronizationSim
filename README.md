# Neuron Synchronization Simulation
The goal of this project is to simulate a graph of coupled neurons.

## Contents
1. [Project Directories](https://github.com/LittleB30/IT287/tree/Dev#project-directories)
2. [Todo](https://github.com/LittleB30/IT287/tree/Dev#todo)

## Project Directories
- Bin: Contains all executable files.
- Graph: Contains all available input graphs.
- Out: Contains all output data files.
- Plot: Contains various plot images created during development.
  - Scripts: Contains some gnuplot scripts to automate plot creation.
- Src: Contains all source code files including headers, implementations, and drivers.

## Todo
- [x] Add ISI calculations for all neurons.
- [ ] Add diffEQ command-line parameter.
- [ ] Add average frequency calculations for all neurons.
- [ ] Breakup complex methods like runRungeKutta.
- [ ] Fix the weird method declaration of getODEs.
- [x] Make the control variable s be calculated using myNeuron (remove s from the graph diagonal).
- [x] Make findSpikes only look for peaks above a certain threshold.
- [x] Update all method comments.
- [x] Rename and split main.c into separate header files.
- [x] Add a larger test graph.
- [ ] Parallelize runRungeKutta where possible.
- [ ] Create more scripts for generating gnuplot images.
- [ ] Create a python script using NetworkX to generate inputtable graphs.
