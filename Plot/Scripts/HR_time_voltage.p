# @author Alex Smith (SmithAlexLee30@gmail.com)
# @date 7/18/22
#
# Usage: gnuplot -c ./HR_time_voltage.p [# of Neurons]

# Verify the number of command-line arguments.
if (ARGC == 1) {
    # Image setup.
    set title "Hindmarsh-Rose (HR) Neuronal Model"
    set xlabel "Time"
    set ylabel "Voltage"
    set terminal png size 1000,500
    set output sprintf("Plot/HR_time_voltage(N=%s).png", ARG1)

    # Get the s value of each neuron. 
    array sValues[ARG1]
    sStrings = system("awk '{print $2}' Out/s_values")  # Read the second column.
    a = 1
    do for [line in sStrings] {                         # Add each value to the array.
        sValues[a] = line
        a = a + 1
    }

    # Plot all the approximations.
    plot for[i = 0:ARG1-1] "Out/approx".i \
        title sprintf("N%d (s=%s)", i, sValues[i+1]) \
        with lines
} else {
    print sprintf("\n\tUsage: -c %s [# of Neurons]\n", ARG0)
}

