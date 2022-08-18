# Neuron Synchronization Simulation
The goal of this project is to simulate a graph of coupled neurons.

## Contents
1. [Dependencies](https://github.com/AlexLeeSmith/NeuronSynchronizationSim/tree/master#dependencies)
2. [Project Directories](https://github.com/AlexLeeSmith/NeuronSynchronizationSim/tree/master#project-directories)
3. [Get Started](https://github.com/AlexLeeSmith/NeuronSynchronizationSim/tree/master#get-started)
4. [Learn More](https://github.com/AlexLeeSmith/NeuronSynchronizationSim/tree/master#learn-more)
5. [Todo](https://github.com/AlexLeeSmith/NeuronSynchronizationSim/tree/master#todo)

## Dependencies
1. gcc: Some C compiler to generate an executable.
2. gnuplot: Used to create graphics of the simulations.
3. make: Used to compile and clean the workspace.
4. python: Some python interpreter to create new graph files.
    - NetworkX: Python module used to generate graphs (pip install networkx).

## Project Directories
- Bin: Contains all executable files.
- Graph: Contains all available input graphs.
- Out: Contains all output data files.
- Plot: Where all plot images will be saved.
  - Examples: Sample plot scripts.
  - Scripts: gnuplot scripts to automate plot creation.
- Src: Contains all source code files including headers, implementations, and drivers.

## Get Started
To begin running this program, compile the source code by running the "make" command. This will generate an executable file in the Bin folder called "driver." To execute this file type "./Bin/driver". This will show you the arguments necessary to run the program. 

### Running the Simulation
Below is an example execution. This simulation runs from x = 0 to 1000 with a step size of 0.1. The cutoff point (or transient) from which we begin to graph the simulation is at x = 500. The simulation is run on a graph of four neurons stored in a file within the Graph directory:
```
$ ./Bin/driver.exe 0 1000 0.1 500 ./Graph/four
Hindmarsh-Rose (HR) neuronal model:
        4 neurons and 10000 steps
        0.011576 seconds elapsed
```

### Plotting the Data
Once the simulation data has been created, we may now draw the graphs to visualize the simulation. All available scripts for plotting the data may be found in the "/Plot/Scripts/" directory. To see how to run each script just type "gnuplot {script_path}". Below you may see the result of running the plot scripts on our data:

```
$ gnuplot -c Plot/Scripts/HR_time_voltage.p 4
```
![Time vs. Voltage of the HR model with 4 neurons](/Plot/Examples/HR_time_voltage(N%3D4).png)

```
$ gnuplot -c Plot/Scripts/HR_neruon_spiketime.p 4
```
![Neuron ID vs. Spike Times of the HR model with 4 neurons](/Plot/Examples/HR_neuron_spiketime(N%3D4).png)

### Creating New Graphs
If you would like to generate additional graph files, then you may use the python script called "Src/graph_builder_grid.py." This script uses the NetworkX library to create a graph, and then write its adjacency matrix to a formatted file. Below is an example execution for a grid of 4x4 neurons with a coupling strength of .2 throughout:
```
$ python Src/graph_builder_grid.py 4 4 .2
Graph has been saved to: Graph/4x4
```

Now, if you navigate to the file saved at "Graph/4x4" you will find the following adjacency matrix:
```
# 16
0 0.2 0 0 0.2 0 0 0 0 0 0 0 0 0 0 0 
0.2 0 0.2 0 0 0.2 0 0 0 0 0 0 0 0 0 0 
0 0.2 0 0.2 0 0 0.2 0 0 0 0 0 0 0 0 0 
0 0 0.2 0 0 0 0 0.2 0 0 0 0 0 0 0 0 
0.2 0 0 0 0 0.2 0 0 0.2 0 0 0 0 0 0 0 
0 0.2 0 0 0.2 0 0.2 0 0 0.2 0 0 0 0 0 0 
0 0 0.2 0 0 0.2 0 0.2 0 0 0.2 0 0 0 0 0 
0 0 0 0.2 0 0 0.2 0 0 0 0 0.2 0 0 0 0 
0 0 0 0 0.2 0 0 0 0 0.2 0 0 0.2 0 0 0 
0 0 0 0 0 0.2 0 0 0.2 0 0.2 0 0 0.2 0 0 
0 0 0 0 0 0 0.2 0 0 0.2 0 0.2 0 0 0.2 0 
0 0 0 0 0 0 0 0.2 0 0 0.2 0 0 0 0 0.2 
0 0 0 0 0 0 0 0 0.2 0 0 0 0 0.2 0 0 
0 0 0 0 0 0 0 0 0 0.2 0 0 0.2 0 0.2 0 
0 0 0 0 0 0 0 0 0 0 0.2 0 0 0.2 0 0.2 
0 0 0 0 0 0 0 0 0 0 0 0.2 0 0 0.2 0 
```

## Learn More
- The simulation uses a set of differential equations (model) to approximate the behavior of a neuron at any given moment. The current model being used is the Hindmarsh-Rose (HR) neuronal model, which consists of a system of three differential equations. This model was altered by adding a coupling factor, which allows the neurons to affect the behavior of adjacent neurons.
- The Runge-Kutta 4 numerical method is used to approximate the values of each equation within the model over each step. This is a fourth-order numerical method, which means at every step a weighted average of four slopes is taken to approximate the next position. This calculation must be done for each differential equation of each neuron before moving to the next step.

## Todo
- [ ] Add diffEQ command-line parameter.
- [ ] Make getS more efficient.
- [ ] Breakup complex methods like runRungeKutta.
- [ ] Fix the weird method declaration of getODEs.
- [ ] Parallelize runRungeKutta where possible.
