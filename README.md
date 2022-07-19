# Neuron Synchronization Simulation
The goal of this project is to simulate a graph of coupled neurons.

## Contents
1. [Dependencies](https://github.com/AlexLeeSmith/NeuronSynchronizationSim/tree/Dev#dependencies)
2. [Project Directories](https://github.com/AlexLeeSmith/NeuronSynchronizationSim/tree/Dev#project-directories)
3. [Todo](https://github.com/AlexLeeSmith/NeuronSynchronizationSim/tree/Dev#todo)

## Dependencies
1. gcc: Some C compiler to generate an executable.
2. gnuplot: Used to create graphics of the simulations.
3. make: Used to compile and clean the workspace.

## Project Directories
- Bin: Contains all executable files.
- Graph: Contains all available input graphs.
- Out: Contains all output data files.
- Plot: Where all plot images will be saved.
  - Examples: Sample plot scripts.
  - Scripts: gnuplot scripts to automate plot creation.
- Src: Contains all source code files including headers, implementations, and drivers.

## Todo
- [ ] Add diffEQ command-line parameter.
- [ ] Add check on calcISI that spike count >= 2.
- [ ] Add check on findSpikes that approx.size >= 3.
- [ ] Make getS more efficient.
- [x] Add average frequency calculations for all neurons to file (neuronNum aveFreq).
- [x] Print s values to a file (neuronNum s)
- [ ] Breakup complex methods like runRungeKutta.
- [ ] Fix the weird method declaration of getODEs.
- [x] Gnuplot script for generating (x approx) images.
- [x] Gnuplot script for generating (neuronNum spikeX) images.
- [ ] Create a python script using NetworkX to generate adjacency matrices.
- [ ] Add logic somewhere to scale the adjacency matrix weights.
- [ ] Parallelize runRungeKutta where possible.
