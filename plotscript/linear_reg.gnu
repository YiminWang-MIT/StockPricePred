set terminal png
set term png size 2400,600
set datafile separator ","

#plot 1
set title ""

#set mytics 5

set autoscale
#set yrange [4130:4160]
set yrange [9060:9140]
#set xrange [4000:4200]
set out "plots/linear_reg_A.png"
plot "data/dataA_linear_reg.csv" using 1 w p title "STOCKA true",\
  "data/dataA_linear_reg.csv" using 2 w p title "STOCKA pred"
#plot "data/dataA_linear_reg.csv" using 1:2 w p title "STOCKA true",\
#  "data/dataA_linear_reg.csv" using 1:3 w p title "STOCKA pred"
unset yrange
unset out

set autoscale
set yrange [4140:4160]
set xrange [4000:5000]
set out "plots/price.png"
plot "data/dataA_int_parsed.csv" using 1:2 w p title "STOCKA bid",\
  "data/dataA_int_parsed.csv" using 1:3 w p title "STOCKA ask"
unset yrange
unset out

