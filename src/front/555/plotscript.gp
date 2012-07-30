set nokey
set size ratio -1
set xrange[0:2.5]
set yrange[0:2.5]
set zrange[0:2.5]
splot "a0.txt" u 1:2:3 w p linecolor 0, "a1.txt" u 1:2:3 w p linecolor 2 lt 2, "a2.txt" u 1:2:3 w p linecolor 1 lt 2, "a4.txt" u 1:2:3 w p linecolor 3 lt 1;
#set term postscript
#set output "t3_555.ps"
#replot
