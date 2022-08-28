set terminal qt 0
set title "Diameter Vs Time"
set grid
set ylabel "Diameter(mm)"
set xlabel "Time(s)"
set xrange [0:4.5]
set yrange [0:2.3]
plot "ButanolDia.txt" using 1:2 title "Butanol" with lines, \
"JetDia.txt" using 1:2 title "JetA1" with lines

set terminal qt 1
set title "(D/D_0)^2 Vs Time"
set grid
set ylabel "(D/D_0)^2"
set xlabel "Time(s)"
set xrange [0:4.5]
set yrange [0:1.2]
plot "ButanolDia.txt" using 1:3 title "Butanol" with lines, \
"JetDia.txt" using 1:3 title "JetA1" with lines
