set title "Exponential ODE"
set xlabel "x"
set ylabel "y"
set terminal png
set output ARG1

plot "Out/numerical_out" title ARG2 w l
