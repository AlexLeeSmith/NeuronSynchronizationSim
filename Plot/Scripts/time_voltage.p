# Image setup.
set title "Hindmarsh-Rose (HR) Neuronal Model"
set xlabel "Time"
set ylabel "Voltage"
set terminal png size 1000,500
set output "Plot/time_voltage.png"

# Get all the approximation files.
files = system("ls -1 Out/approx*")

# Get the index of the neuron number.
numIndex = strstrt(files, "0")

# Plot all the approximations.
plot for [data in files] data \
    title "Neuron ".data[numIndex:] \
    with lines