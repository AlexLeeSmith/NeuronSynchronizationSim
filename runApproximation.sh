# Usage: ./runApproximation.sh [Method] [plotScript.p] [imageName.png]
# Methods: Eulers, RungeKutta
#
# @author Alex Smith (alsmi14@ilstu.edu)
# @date 2/17/22

if [ $# -eq 3 ]
then
    binFolder='Bin'
    outFolder='Out'
    plotFolder='Plot'
    numericalBin=$binFolder/'numerical_methods'
    numericalOut=$outFolder/"numerical_out"
    imagePlot=$plotFolder/$3

    make cleanRunApproximation
    make numerical

    ./$numericalBin $1 > $numericalOut
    gnuplot -c $2 $imagePlot $1
else
    printf "\n\tUsage: ./runApproximation.sh [Method] [plotScript.p] [imageName.png]\n"
    printf "\tMethods: Eulers, RungeKutta\n\n"
fi
