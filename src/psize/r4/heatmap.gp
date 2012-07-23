set nokey
set size ratio -1
set pm3d map
set term png
set palette defined(0 "blue", 200 "white", 500 "red")
set output "00.png"
splot "00.txt"
