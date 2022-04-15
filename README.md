# IT287
The goal of this project is to simulate a graph of coupled neurons.

## Project Directories
- Bin: Contains all executable files.
- Graph: Contains all availible input graphs.
- Out: Contains all output data files.
- Plot: Contains various plot images created during development.
  - Scripts: Contains some gnuplot scripts to automate plot creation.
- Src: Contains all source code files including headers, implementations, and drivers.

## Todo
- [ ] Add ISI calculations for all neurons.
- [ ] Breakup complex methods like runRungeKutta.
- [ ] Make the control variable s be calculated using myNeuron (remove s from the graph diagonal).
- [ ] Make findSpikes only look for peaks above a certain threshold.
- [ ] Update all method comments.
- [ ] Parallelize runRungeKutta where possible.
- [ ] Create more scripts for generating gnuplot images.
- [ ] Create a python script using NetworkX to generate inputtable graphs.
