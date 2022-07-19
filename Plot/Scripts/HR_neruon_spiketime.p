# @author Alex Smith (SmithAlexLee30@gmail.com)
# @date 7/18/22
#
# Usage: gnuplot -c ./HR_neuron_spiketime.p [# of Neurons]

# Verify the number of command-line arguments.
if (ARGC == 1) {
    # Image setup.
    unset key
    set title "Hindmarsh-Rose (HR) Neuronal Model"
    set xlabel "Spike Time"
    set ylabel "Neuron ID"
    set yrange [-0.5:ARG1-0.5]
    set ytics 0, 1
    set terminal png size 1000,500
    set output sprintf("Plot/HR_neuron_spiketime(N=%s).png", ARG1)

    # Plot the spike timings of each neuron.
    plot for[i = 0:ARG1-1] "Out/spikes".i using 1:(i) \
        with points pointsize 2
} else {
    print sprintf("\n\tUsage: gnuplot -c %s [# of Neurons]\n", ARG0)
}

