set title "Hindmarsh-Rose neuronal model (s=4.2)"
set xlabel "Time"
set ylabel "Voltage"
set terminal png size 1000,500
set output ARG1

plot "Out/numerical_out" title ARG2 w l
