set terminal postscript enhanced color
set size 0.5,0.5
set output "pareto.ps"
set xrange [0:4]
set yrange [0:4]
plot "points.dat" title "Non-optimal points" with points,\
     "optimal.dat" title "Pareto-optimal set of points" with linespoints
set xlabel "Throughput 1"
set ylabel "Throughput 2"
