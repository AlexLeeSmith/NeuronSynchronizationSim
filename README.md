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
- [ ] Add diffEQ command-line parameter.
- [ ] Add check on calcISI that spike count >= 2.
- [ ] Add check on findSpikes that approx.size >= 3.
- [ ] Make getS more efficient.
- [ ] Add average frequency calculations for all neurons to file (neuronNum aveFreq).
- [ ] Print s values to a file (neuronNum s)
- [ ] Breakup complex methods like runRungeKutta.
- [ ] Fix the weird method declaration of getODEs.
- [ ] Create more scripts for generating gnuplot images (x approx) (neuronNum spikeX).
- [ ] Create a python script using NetworkX to generate adjacency matrices.
- [ ] Add logic somewhere to scale the adjacency matrix weights.
- [ ] Parallelize runRungeKutta where possible.
