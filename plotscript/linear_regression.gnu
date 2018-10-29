set terminal png
set term png size 2400,600
set datafile separator ","

#plot 1
set title ""

#set mytics 5

set autoscale
set yrange [4140:4160]
set xrange [200:500]
set out "plots/linear_A.png"
plot "data/dataA_test_res.csv" using 1 w p title "STOCKA ture",\
  "data/dataA_test_result.csv" using 1 w p title "STOCKA predict"
#plot "data/dataA_test_compare.csv" using 1:2 w p title "STOCKA ture"
unset yrange
unset out

set autoscale
set xrange [1.6e10:1.7e10]
set out "plots/linearA_time.png"
plot "data/dataA_test_result_combined.csv" using 1:2 w p title "STOCKA ture",\
  "data/dataA_test_result_combined.csv" using 1:3 w p title "STOCKA predict"
#plot "data/dataA_test_compare.csv" using 1:2 w p title "STOCKA ture"
unset yrange
unset out
