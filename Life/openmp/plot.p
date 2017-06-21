# Reference https://stackoverflow.com/questions/19201484/gnuplot-plot-2d-matrix-with-image-want-to-draw-borders-for-each-cell
set size ratio 1
set palette gray negative
set autoscale xfix
set autoscale yfix
set xtics 1
set ytics 1
set title "Resolution Matrix for E"

set tics scale 0,0.0001
set mxtics 2
set mytics 2
set grid front mxtics mytics lw 0.2 lt -1 lc rgb 'black'
plot "dim_200_gen_10.dat" matrix w image noti
