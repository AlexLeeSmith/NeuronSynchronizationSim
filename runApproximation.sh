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
    mainBin=$binFolder/'main'
    mainOut=$outFolder/"runApproximation"
    imagePlot=$plotFolder/$3

    make allclean

    ./$mainBin $1 > $mainOut
    gnuplot -c $2 $imagePlot $1
else
    printf "\n\tUsage: ./runApproximation.sh [Method] [plotScript.p] [imageName.png]\n"
    printf "\tMethods: Eulers, RungeKutta\n\n"
fi
